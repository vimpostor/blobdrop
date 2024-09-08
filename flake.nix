{
	description = "Drag and drop your files directly out of the terminal";
	inputs = {
		quartz.url = "github:vimpostor/quartz";
	};

	outputs = { self, quartz }: quartz.lib.eachSystem (system:
		let
			pkgs = quartz.inputs.nixpkgs.legacyPackages.${system};
			stdenvs = [ { name = "gcc"; pkg = pkgs.gcc14Stdenv; } { name = "clang"; pkg = pkgs.llvmPackages_18.stdenv; } ];
			defaultStdenv = (builtins.head stdenvs).name;
			quartzCode = pkgs.fetchFromGitHub {
				owner = "vimpostor";
				repo = "quartz";
				rev = builtins.head (builtins.match ".*FetchContent_Declare\\(.*GIT_TAG ([[:alnum:]\\.]+).*" (builtins.readFile ./CMakeLists.txt));
				hash = "sha256-N4wdoZx6sQejfy/9tqtQIOcT9q9fB1DnSSnegnWDtXo=";
			};
			makeStdenvPkg = env: env.mkDerivation {
				pname = "blobdrop";
				version = builtins.head (builtins.match ".*project\\([[:alnum:]]+ VERSION ([0-9]+\.[0-9]+).*" (builtins.readFile ./CMakeLists.txt));

				src = ./.;

				nativeBuildInputs = with pkgs; [
					cmake
					pkg-config
					qt6.wrapQtAppsHook
				];
				buildInputs = with pkgs; [
					qt6.qtbase
					qt6.qtdeclarative
					qt6.qtsvg
					xorg.libxcb
					xorg.xcbutilwm
				];

				cmakeFlags = [("-DFETCHCONTENT_SOURCE_DIR_QUARTZ=" + quartzCode)];
			};
		in {
			packages = {
				default = self.outputs.packages.${system}.${defaultStdenv};
			} // builtins.listToAttrs (map (x: { name = x.name; value = makeStdenvPkg x.pkg; }) stdenvs);
			checks = {
				format = pkgs.runCommand "format" { src = ./.; nativeBuildInputs = [ pkgs.clang-tools pkgs.git ]; } "mkdir $out && cd $src && find . -type f -path './*\\.[hc]pp' -exec clang-format -style=file --dry-run --Werror {} \\;";
			} // builtins.listToAttrs (map (x: { name = "tests-" + x.name; value = (makeStdenvPkg x.pkg).overrideAttrs (finalAttrs: previousAttrs: {
					doCheck = true;
					cmakeFlags = previousAttrs.cmakeFlags ++ ["-DBUILD_TESTING=ON"];
					QT_QPA_PLATFORM = "offscreen";
				}
			); }) stdenvs);
		}
	);
}

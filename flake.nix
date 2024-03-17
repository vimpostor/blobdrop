{
	description = "Drag and drop your files directly out of the terminal";
	inputs = {
		nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
	};

	outputs = { self, nixpkgs }:
	let eachSystem = g: (a: nixpkgs.lib.genAttrs (builtins.attrNames (builtins.getAttr (builtins.head (builtins.attrNames a)) a)) (d: nixpkgs.lib.genAttrs (builtins.attrNames a) (s: a.${s}.${d}))) (nixpkgs.lib.genAttrs nixpkgs.lib.systems.flakeExposed (y: g y));
	in eachSystem (system:
		let
			pkgs = nixpkgs.legacyPackages.${system};
			stdenvs = [ { name = "gcc"; pkg = pkgs.gcc13Stdenv; } { name = "clang"; pkg = pkgs.llvmPackages_17.stdenv; } ];
			defaultStdenv = (builtins.head stdenvs).name;
			quartz = pkgs.fetchFromGitHub {
				owner = "vimpostor";
				repo = "quartz";
				rev = builtins.head (builtins.match ".*FetchContent_Declare\\(.*GIT_TAG ([[:alnum:]\\.]+).*" (builtins.readFile ./CMakeLists.txt));
				hash = "sha256-gni+5kQkjU1LhoEK6/yVC0x6K36Ra30xM47w893nGJ4=";
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

				cmakeFlags = [("-DFETCHCONTENT_SOURCE_DIR_QUARTZ=" + quartz)];
			};
		in {
			packages = {
				default = self.outputs.packages.${system}.${defaultStdenv};
			} // builtins.listToAttrs (map (x: { name = x.name; value = makeStdenvPkg x.pkg; }) stdenvs);
			checks = {
				format = pkgs.runCommand "format" { src = ./.; nativeBuildInputs = [ pkgs.clang-tools pkgs.git ]; } "mkdir $out && cd $src && find . -type f -path './*\\.[hc]pp' -exec clang-format -style=file --dry-run --Werror {} \\;";
				tests = (makeStdenvPkg pkgs.gcc13Stdenv).overrideAttrs (finalAttrs: previousAttrs: {
					doCheck = true;
					cmakeFlags = previousAttrs.cmakeFlags ++ ["-DBUILD_TESTING=ON"];
					QT_QPA_PLATFORM = "offscreen";
				});
			};
		}
	);
}

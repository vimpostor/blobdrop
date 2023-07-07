{
	description = "Drag and drop your files directly out of the terminal";
	inputs = {
		nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
		flake-utils.url = "github:numtide/flake-utils";
	};

	outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
		let
			pkgs = nixpkgs.legacyPackages.${system};
		in {
			packages = rec {
				default = blobdrop;

				blobdrop = pkgs.stdenv.mkDerivation {
					pname = "blobdrop";
					version = "1.0";

					src = ./.;

					nativeBuildInputs = with pkgs; [
						cmake
						pkgconfig
						qt6.wrapQtAppsHook
					];

					buildInputs = with pkgs; [
						qt6.qtbase
						qt6.qtdeclarative
					];
				 };
			};
		}
	);
}

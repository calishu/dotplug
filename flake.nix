{
    description = "dev env";

    inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

    outputs = { self, nixpkgs }:
        let
            systems = [ "x86_64-linux" "x86_64-darwin" ];
        in
        {
            devShells = builtins.listToAttrs (map (system:
            {
                name = system;
                value = let pkgs = import nixpkgs { inherit system; }; in {
                    default = pkgs.mkShell {
                        buildInputs = [
                          pkgs.clang
                          pkgs.cmake
                          pkgs.meson
                          pkgs.ninja
                          pkgs.python310
                          pkgs.pkg-config
                          pkgs.catch2_3
                          pkgs.nlohmann_json
                          pkgs.cli11
                        ];
                        shellHook = ''
                            export CC=clang
                            export CXX=clang++
                        '';
                    };
                };
            }) systems);
        };
}

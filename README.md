# DotPlug

> [!NOTE]
> This project is actively under development and requires further work and testing.

DotPlug is a user-friendly dotfile manager designed to help you organize and manage your favorite configuration files effortlessly.

## Table of Content
- [Installation](#installation)
⠀ - [Build from Source](#build-from-source)
- [Getting Started](#getting-started)
⠀ - ⠀[Base Commands](#base-commands)
⠀ - ⠀[Config Commands](#config-commands)
- [For Contributors](#for-contributors)
- [License](#license)

## Installation
### Build from Source
To build **dotplug** from source, make sure you have [`git`](https://git-scm.com/), [`meson`](https://mesonbuild.com/), and [`conan`](https://conan.io/) installed on your system.
```bash
git clone https://github.com/calishu/dotplug.git
cd dotplug
conan install . --output-folder=build --build=missing
meson setup build
meson compile -C build
```
If you want to reconfigure the meson build using the `--reconfigure` flag, the meson script will install the Conan dependencies automatically for you.

## Getting Started
#### Base Commands
| Command           | Action |
| ---               | ---    |
| `dotplug list`    | Shows you a list of all configurations. |
| `dotplug init`    | Initializes a new configuation. |
| `dotplug install` | Installs a new configuration from a remote repository. |
| `dotplug config`  | Takes actions with a existing configuration. |

#### Config Commands
The `dotplug config` command takes the name parameter, followed by a sub command.
| Sub Command | Action |
| ---         | ---    |
| `validate`  | Validates the `config.toml` by the configuration. |
| `remove`    | Deletes the configuration from the drive. |
| `apply`     | Applies the selected configuration. |
| `show`      | Shows the meta data from said configuration. |

## For Contributors
* Interested in contributing? Check out our [**Contribution Guide**](https://github.com/calishu/dotplug/tree/main/.github/CONTRIBUTING.md) for guidelines and best practices.
* To foster a welcoming community, please read our [**Code of Conduct**](https://github.com/calishu/dotplug/tree/main/.github/CODE_OF_CONDUCT.md).
* Found a bug or have a feature request? Feel free to open an [**Issue**](https://github.com/calishu/dotplug/issues). We'd love to hear from you!

## License
DotPlug is open source and distributed under the [MIT License](https://github.com/calishu/dotplug/tree/main/LICENSE).

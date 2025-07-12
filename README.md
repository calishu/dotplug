# DotPlug

> [!NOTE]
> This project is actively under development and requires further work and testing.

DotPlug is a user-friendly dotfile manager designed to help you organize and manage your favorite configuration files effortlessly.

## Installation

### Build from Source
To build **dotplug** from source, make sure you have [`git`](https://git-scm.com/), [`meson`](https://mesonbuild.com/), and [`conan`](https://conan.io/) installed on your system.
```bash
git clone https://github.com/calishu/dotplug.git
cd dotplug
meson setup build
meson compile -C build
```
Conan will automatically install the dependencies thanks to the meson script.

## For Contributors

* Interested in contributing? Check out our [**Contribution Guide**](https://github.com/calishu/dotplug/tree/main/.github/CONTRIBUTING.md) for guidelines and best practices.
* To foster a welcoming community, please read our [**Code of Conduct**](https://github.com/calishu/dotplug/tree/main/.github/CODE_OF_CONDUCT.md).
* Found a bug or have a feature request? Feel free to open an [**Issue**](https://github.com/calishu/dotplug/issues). We'd love to hear from you!

## License

DotPlug is open source and distributed under the [MIT License](https://github.com/calishu/dotplug/tree/main/LICENSE).

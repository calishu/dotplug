#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "context.hpp"
#include "settings.hpp"
#include "utils/config.hpp"
#include "utils/symlinks.hpp"

int apply()
{
	if (std::filesystem::exists(config_path + "current"))
		remove_all_symlinks();

	const Config config = Config(ctx->name);

	for (std::string &dep_name : config.get_dependencies())
	{
		std::unordered_map<std::string, std::string> dep = config.get_dependency(dep_name);

		if (!std::filesystem::exists(dep["source"]))
		{
			std::cerr << "[ERROR] The source of " << dep_name << " couldn't be found!" << std::endl;
			return 1;
		}
		if (dep["destination"].empty())
		{
			std::cerr << "[ERROR] Couldn't find the destination of " << dep_name << std::endl;
			return 1;
		}

		std::error_code err;
		std::filesystem::create_directory_symlink(dep["source"], dep["destination"], err);

		if (err)
			throw std::runtime_error(err.message());
	}

	std::fstream current(config_path + "current");
	current << (dotfiles_path + config.name_ + "/config.toml");
	current.close();

	return 0;
}

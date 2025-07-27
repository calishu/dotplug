#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include <git2.h>
#include <git2/repository.h>

int create_commit(const std::string &repo_path, const std::string &config_path, const std::string &description) {
    git_oid tree_id, commit_id;
    git_repository *repo      = nullptr;
    git_index *index          = nullptr;
    git_tree *tree            = nullptr;
    git_signature *sig        = nullptr;
    git_commit *parent_commit = nullptr;

    if (git_repository_open_ext(&repo, repo_path.c_str(), GIT_REPOSITORY_OPEN_NO_SEARCH, config_path.c_str()) != 0) {
        std::cerr << "[ERROR] Failed to open repo :<\n";
        return 1;
    }

    if (git_repository_index(&index, repo) != 0) {
        std::cerr << "[ERROR] Failed to open index\n";
        return 1;
    }

    if (git_index_add_all(index, nullptr, GIT_INDEX_ADD_DEFAULT, nullptr, nullptr) != 0) {
        std::cerr << "[ERROR] Failed to add files in index\n";
        return 1;
    }

    if (git_index_write(index) != 0) {
        std::cerr << "[ERROR] Failed to write index\n";
        return 1;
    }

    if (git_index_write_tree(&tree_id, index) != 0) {
        std::cerr << "[ERROR] Failed to write tree\n";
        return 1;
    }

    if (git_tree_lookup(&tree, repo, &tree_id) != 0) {
        std::cerr << "[ERROR] Failed to lookup tree\n";
        return 1;
    }

    std::string username = getlogin();
    if (git_signature_now(&sig, username.c_str(), nullptr) != 0) {
        std::cerr << "[ERROR] Failed to create signature\n";
        return 1;
    }

    if (git_reference_name_to_id(&commit_id, repo, "HEAD") == 0)
        git_commit_lookup(&parent_commit, repo, &commit_id);

    if (git_commit_create_v(
            &commit_id,
            repo,
            "HEAD",
            sig,
            sig,
            nullptr,
            description.c_str(),
            tree,
            parent_commit ? 1 : 0,
            parent_commit ? (const git_commit **)&parent_commit : nullptr) != 0) {
        std::cerr << "[ERROR] Failed to create backup\n";
        return 1;
    }

    git_commit_free(parent_commit);
    git_signature_free(sig);
    git_tree_free(tree);
    git_index_free(index);
    git_repository_free(repo);

    return 0;
}

#pragma once
#include <string>

namespace info {

std::string info_project_author();
std::string info_project_repository_url();
std::string info_project_repository_branch();
std::string info_project_revision();
std::string info_project_revision_datetime();
std::string info_project_revision_history();
std::string info_project_version();
std::string info_project_full();

} // namespace info
#pragma once
#include <string>

namespace info {

const char *info_project_author();
const char *info_project_repository_url();
const char *info_project_repository_branch();
const char *info_project_revision();
const char *info_project_revision_datetime();
const char *info_project_revision_history();
const char *info_project_version();
std::string info_project_full();

} // namespace info
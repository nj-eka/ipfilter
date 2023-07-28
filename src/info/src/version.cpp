#include <info/config.h>
#include <info/version.h>
#include <sstream>

namespace info {

std::string info_project_author() { return PROJECT_AUTHOR; }
std::string info_project_repository_url() { return PROJECT_REPOSITORY_URL; }
std::string info_project_repository_branch() { return PROJECT_REPOSITORY_BRANCH; }
std::string info_project_revision() { return PROJECT_REVISION; }
std::string info_project_revision_datetime() { return PROJECT_REVISION_DATETIME; }
std::string info_project_revision_history() { return PROJECT_REVISION_HISTORY; }
std::string info_project_version() { return PROJECT_VERSION; }

std::string info_project_full() {
  std::stringstream ss;
  ss << "author: " << info_project_author() << std::endl;
  ss << "url: " << info_project_repository_url() << std::endl;
  ss << "branch: " << info_project_repository_branch() << std::endl;
  ss << "rev: " << info_project_revision() << std::endl;
  ss << "rev date: " << info_project_revision_datetime() << std::endl;
  ss << "rev hist: " << info_project_revision_history() << std::endl;
  return ss.str();
}

} // namespace info

#include "init/init.h"

#include "utility/utility.h"

void CheckConfigFolder() {
  const auto strRootPath = Lee::GetRootPath();
  const std::string strConfigPath =
      strRootPath + "\\" + Lee::DEFAULT_CONFIG_FOLDER_NAME;
  const std::string strConfigPathAndName =
      strConfigPath + "\\" + Lee::DEFAULT_CONFIG_FILE_NAME;
  /** 检查并创建程序所需要的配置文件和文件夹 */
  /** 首先检查配置文件的路径有没有 */
  if (!Lee::IsFileExist(strConfigPath)) {
    if (!Lee::CreateFileFolder(strConfigPath)) {
      std::cout << "Path " << strConfigPath << " is not exist!" << std::endl;
      assert(false && "can't create file floder");
      exit(-1);
    }
    /** TODO: 这里面只创建了config文件夹，没创建conf.ini */
  }
  /** 检查配置文件的存在与否 */
  if (!Lee::IsFileExist(strConfigPathAndName)) {
    std::cout << "file " << strConfigPathAndName << " is not exist!"
              << std::endl;
    assert(false && "can't create file.");
    exit(-1);
    /** TODO: 这个分支可以考虑创建一个默认的配置文件 */
  }
}

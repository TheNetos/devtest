#ifndef APPCORE_HPP
#define APPCORE_HPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "faceCV/faceCV.hpp"

namespace devtest {

namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace ptree = boost::property_tree;

/**
 * @brief Core application class
 * 
 */
class AppCore
{
public:
    /**
     * @brief Construct a new App Core object
     * 
     * @param argc 
     * @param argv 
     */
    AppCore(const int argc, const char *argv[]);

    /**
     * @brief Destroy the App Core object
     * 
     */
    ~AppCore();

    /**
     * @brief Launches application execution
     * 
     * @return int application return code
     */
    int exec();

private:
    /**
     * @brief Set of general options for the application
     * 
     */
    po::options_description mOptGeneral;

    /**
     * @brief Set of additional options for the application
     * 
     */
    po::options_description mOptAdditional;

    /**
     * @brief A map of parsed options for the application
     * 
     */
    po::variables_map mOptMap;

    /**
     * @brief The path for the directory with the input dataset
     * 
     */
    std::string mInputPath;

    /**
     * @brief Valid file types to process
     * 
     */
    std::vector<std::string> mInputTypes {".jpg",".jpeg",".png"};

    /**
     * @brief A set of file paths to process
     * 
     */
    std::vector<fs::path> mFilePaths;

    /**
     * @brief Absolute application path
     * 
     */
    fs::path mAppPath;

    /**
     * @brief Target directory scan
     * 
     * Reads a list of files for processing from the target directory 
     * specified in the input parameters.
     * The result is placed in AppCore#mFilePaths.
     * 
     * @return std::size_t Number of files found for processing
     */
    std::size_t scanTargetDir();

    /**
     * @brief Handles found files
     * 
     * @return double Processing time
     */
    double computeFaces();
};

}


#endif //APPCORE_HPP

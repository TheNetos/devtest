#include "AppCore.hpp"

using namespace devtest;

AppCore::AppCore(const int argc, const char *argv[]) : mOptGeneral("General options"), mOptAdditional("Additional options")
{
    // Define app options
    mOptGeneral.add_options()
        ("help,h", "Show help")
        ("input,I", po::value<std::string>(), "The path to the dataset to process");
    mOptAdditional.add_options()
        ("types,T", po::value<std::vector<std::string>>(), "Setting valid file types for reading (default jpg,jpeg,png)");
    po::options_description optAll("Allowed options");
    optAll.add(mOptGeneral).add(mOptAdditional);

    // Try parse command line
    try
    {
        po::store(po::parse_command_line(argc, argv, optAll), mOptMap);
        po::notify(mOptMap);
    }
    catch (std::exception &e)
    {
        std::cerr << "Arguments error: " << e.what() << std::endl;
    }
    
    mAppPath = boost::filesystem::system_complete(argv[0]).parent_path();
}

AppCore::~AppCore() {}

int AppCore::exec()
{
    if (mOptMap.count("help"))
    {
        std::cout << mOptGeneral << std::endl
                  << mOptAdditional << std::endl;
        return 0;
    }

    int retCode = -1;

    try
    {
        // Processing command line args
        if (mOptMap.count("input"))
        {
            const std::string &inputPath = mOptMap["input"].as<std::string>();
            if (!fs::exists(inputPath) || !fs::is_directory(inputPath))
            {
                throw "Input path '" + inputPath + "' not found";
            }
            mInputPath = inputPath;
            std::cout << "Set root path for process: '" << inputPath << "'" << std::endl;
        }
        else
        {
            throw "You must specify the path to the incoming dataset! (--input <path>)";
        }

        if (mOptMap.count("types")) {
            const std::vector<std::string> &inputTypes = 
                mOptMap["types"].as< std::vector<std::string> >();
            if (!inputTypes.empty()) {
                mInputTypes.clear();
                std::vector<std::string> tempSplit;
                for(const std::string &str: inputTypes) {
                    boost::algorithm::split(tempSplit, str, boost::is_any_of(","));
                    mInputTypes.insert(mInputTypes.end(), tempSplit.begin(), tempSplit.end());
                }
            }
        }

        if (!mInputTypes.empty()) {
            // Normalize extensions strings
            for (std::string &str: mInputTypes) {
                boost::algorithm::to_lower(str);
                if (str.at(0) != '.') {
                    str = "." + str;
                }
            }
            // Remove duplicates
            std::set<std::string> normDup(mInputTypes.begin(), mInputTypes.end());
            mInputTypes.assign(normDup.begin(), normDup.end());
        }

        // Scan target dir
        if (this->scanTargetDir() == 0)
        {
            throw "No files found along path '" + mInputPath + "' with extensions: " + 
                  boost::algorithm::join(mInputTypes, ", ");
        }

        std::cout << "Found " << mFilePaths.size() << " files" << std::endl;

        // Reading the cascade file and calling the recognition function
        double computeTime = this->computeFaces();
        std::cout << "Processing time for all files: " << computeTime << std::endl;

        std::cout << "Done." << std::endl;
        retCode = 0;
    }
    catch (const std::string &e)
    {
        std::cerr << e << std::endl;
    }
    catch (const char *e)
    {
        std::cerr << e << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return retCode;
}

std::size_t AppCore::scanTargetDir()
{
    boost::system::error_code ec;
    fs::recursive_directory_iterator it(mInputPath, ec);
    fs::recursive_directory_iterator itEnd;
    
    std::cout << "Running scan in directory '" << mInputPath << "' with extensions: "
              << boost::algorithm::join(mInputTypes, ", ") << std::endl;
    bool isExt = false;

    for(; it != itEnd; it.increment(ec)) {
        if (ec) {
            // If dir access denied
            it.pop();
            continue;
        }

        isExt = std::find(
                    mInputTypes.cbegin(),
                    mInputTypes.cend(),
                    it->path().extension() ) != mInputTypes.cend();
        if (fs::is_regular_file(*it) && isExt)
        {
            mFilePaths.push_back(fs::absolute(it->path()));
        }
    }

    std::cout << "Scan done" << std::endl;
    
    return mInputTypes.size();
}

double AppCore::computeFaces() {
    cv::CascadeClassifier cascade;
    std::string cascadePath = (mAppPath / "haarcascade_frontalface_alt2.xml").string();
    std::cout << "Load face recognition cascade file from '" << cascadePath << "'" << std::endl;
    if (!cascade.load(cascadePath))
    {
        throw "Error reading cascade file!";
    }
    
    ptree::ptree resultJSON;
    std::vector<cv::Rect> faceRects;

    struct timespec computeStart, computeEnd;
    double elapsedTime;
    // Mark start
    clock_gettime(CLOCK_MONOTONIC, &computeStart);
    for (const fs::path &path : mFilePaths)
    {
        faceRects = devtest::components::faceCV(path.string(), cascade);

        // Выводим наиденное в древо JSON
        ptree::ptree imgNode;
        if (faceRects.size()) {
            for (const cv::Rect &rect: faceRects) {
                ptree::ptree rectNode;
                rectNode.put("x", rect.x);
                rectNode.put("y", rect.y);
                rectNode.put("width", rect.width);
                rectNode.put("height", rect.height);
                imgNode.push_back(std::make_pair("", rectNode));
            }
        } else {
            imgNode.put_value("#[]#"); 
        }
        
        resultJSON.push_back(std::make_pair(path.string(), imgNode));
    }
    clock_gettime(CLOCK_MONOTONIC, &computeEnd);  // Mark end
    
    std::ofstream jsonOutSream;
    fs::path jsonOutPath(mInputPath);
    jsonOutPath /= "result.json";
    jsonOutSream.open(jsonOutPath.string());
    // Не самое красивое решение проблемы вывода "" вместо []
    std::stringstream jsonOutStringStream;
    ptree::json_parser::write_json(jsonOutStringStream, resultJSON, true);
    std::string jsonOutString = jsonOutStringStream.str();
    boost::replace_all(jsonOutString, "\"#[]#\"", "[]");
    jsonOutSream << jsonOutString;
    jsonOutSream.close();

    elapsedTime = (computeEnd.tv_sec - computeStart.tv_sec);
    elapsedTime += (computeEnd.tv_nsec - computeStart.tv_nsec) / 1000000000.0;
    return elapsedTime;
}

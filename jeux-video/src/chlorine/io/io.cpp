#include <chlorine/io/io.hpp>
#include <chlorine/utils/strings.hpp>


namespace chlorine::io
{

    bool sceneImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::string pathPrefix, std::string filePath, std::map<std::string, std::function<const std::type_info&(std::unique_ptr<::chlorine::scene::component>&)>> mapSwitcher, std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {
        // open the scene file
        std::fstream sceneFile;
        sceneFile.open(filePath, std::ios::in);
        if(sceneFile.is_open() == false)
        {
            logOut->log("Could not open file: " + filePath);
            return false;
        }

        // Every line is split up as follows:
        // [name of component/thingy] [value/path to component]
        std::string firstLine; // name of scene
        std::vector<std::string> splitFirstLine;
        std::getline(sceneFile, firstLine);
        ::chlorine::utils::splitStringToVector(firstLine, splitFirstLine, ' ');
        sceneIn->sceneName = splitFirstLine[1];

        // TODO: whilst still on scene data


        std::string nextLine;
        while(std::getline(sceneFile, nextLine))
        {
            std::vector<std::string> tempSplit;
            ::chlorine::utils::splitStringToVector(nextLine, tempSplit, ' ' );
            std::string componentPath = pathPrefix + sceneIn->sceneName + "/" + tempSplit[1];
            if(tempSplit[0] == "component")
            {
                // sceneIn->components.insert(std::make_pair(tempSplit[1], std::unique_ptr<::chlorine::scene::component>()));
                // logOut->log("Made component: " + tempSplit[1] + '\n');
                std::fstream componentFile;
                componentFile.open(componentPath, std::ios::in);
                if(componentFile.is_open() == false)
                {
                    logOut->log("Cold not open component file: " + componentPath);
                }

                std::string componentFileLine, componentName;
                std::getline(componentFile, componentFileLine);
                std::vector<std::string> componentFileSplit;
                ::chlorine::utils::splitStringToVector(componentFileLine, componentFileSplit, ' '); // 0 = "type", 1 = the actual type name
                std::getline(componentFile, componentName);

                componentFile.close();

                std::unique_ptr<::chlorine::scene::component> tempPtr;
                const std::type_info& tempTypeInfo = mapSwitcher[componentFileSplit[1]](tempPtr);
                // Load file function
                sceneIn->Conductor.instruments[componentName].emplace(tempTypeInfo, std::move(tempPtr));

            }
            else if(tempSplit[0] == "xpos")
            {
                sceneIn->windowSize.first = std::stoi(tempSplit[1]);
            }
            else if(tempSplit[0] == "ypos")
            {
                sceneIn->windowSize.first = std::stoi(tempSplit[1]);
            }
        }

        sceneFile.close();

        return true;
    }
}

#include <chlorine/scene/componentManager.hpp>

namespace chlorine::scene
{

    // Note: this has already been defined in the hpp file, cause templated func :P
    /*
      template<typename T, typename ...Args>
      void insertElement(std::string key1, Args ...args)
      {
      std::unique_ptr<::chlorine::scene::component> tempComponent = std::make_unique<T>(args...);
      instruments[key1].emplace(std::type_index(typeid(T)), std::make_unique<T>(args...));
      _groups[std::type_index(typeid(T))].insert(key1);
      }
    */

    void orchestra::removeElement(std::string key, std::type_index dataType, std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {
        instruments[key].erase(dataType);
        // std::erase(_groups[dataType], key);

        logOut->log("Removed element " + key + " " + dataType.name() + '\n');

    }

    void orchestra::groupAllElements( std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {
        _groups.clear();
        for(const auto & [componentName, instrumentComponentMap] : instruments)
        {
            for(const auto & [instrumentCompName, instrumentcompVal] : instrumentComponentMap)
            {
                _groups[instrumentCompName].insert(componentName);
            }
        }

        logOut->log("Cleared & Regrouped all elements\n");
    }

    std::size_t orchestra::getNumElements()
    {
        return _groups.size();
    }

    std::size_t orchestra::getElementTypeNum(std::type_index type)
    {
        return _groups[type].size();
    }

    std::set<std::string> orchestra::getElementType(std::type_index type)
    {
        return _groups[type];
    }
}

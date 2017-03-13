#include "VAOFactory.h"
#include <iostream>
#include <pybind11/pybind11.h>


namespace ngl
{
  std::unordered_map<std::string, std::function<AbstractVAO *(GLenum _mode)>> VAOFactory::m_vaoCreators;

  void VAOFactory::registerVAOCreator(const std::string &_type,std::function<AbstractVAO *(GLenum _mode)> _cb)
  {
    m_vaoCreators[_type] = _cb;
  }

  void VAOFactory::unregisterVAOCreator(const std::string &_type)
  {
    m_vaoCreators.erase(_type);
  }

  AbstractVAO *VAOFactory::createVAO(const std::string &_type, GLenum _mode)
  {

    auto it = m_vaoCreators.find(_type);
    if (it != m_vaoCreators.end())
    {
      // call the creation callback to construct this derived type
      return it->second(_mode);
    }
    return nullptr;
  }


  void VAOFactory::listCreators()
  {
    std::cout<<"******************************\n";
    std::cout<<"VAOFactory Creators List \n";
    std::cout<<"******************************\n";
    for(auto c : m_vaoCreators)
    {
      std::cout<<"Creator "<<c.first<<" registered \n";
    }
    std::cout<<"******************************\n";
  }

  namespace py = pybind11;

  void pyInitVAOFactory(py::module & m)
  {
    py::class_<VAOFactory>(m, "VAOFactory")
      .def_static("registerVAOCreator",&VAOFactory::registerVAOCreator)
      .def_static("unregisterVAOCreator",&VAOFactory::unregisterVAOCreator)
      .def_static("createVAO",&VAOFactory::createVAO)
      .def_static("listCreators",&VAOFactory::listCreators)
        ;

  }

}

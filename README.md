## This Plugin

An data plugin that exhibits inter-repo dependencies. This is repo is a dependency 
for FooDataLoader. The underlying data is represents an array of complex numbers as std::vector<std::complex<float>>

Together they act as a test for the resolution of inter-repo dependency
resolution using our orchestation tools on github CI (ManiVaultStudio/github-actions)
and in desktop development (DevBundle) 

## Manivault data model for a new data type

```mermaid
%%{init: {"fontFamily": "Helvetica"}}%%

classDiagram

note for AbstractDataManager "Acessed by <br/> mv\:\:data()"
note for AbstractDataHierarchyManager "Accessed by <br/> mv\:\:dataHierarchy()"
note for Dataset "A handle. <br/>Passed between <br/> plugins. Not<br/>a smart pointer"
note for MyData "Holds the data <br/> of type D<br/> Registers a <br/>unique logical <br/>mv\:\:DataType"

namespace mv {
 class AbstractDataManager {
  createDataSet~T~(type, name, parentDataset) "hierarchy parentage"
  createDerivedDataset() Dataset~DatasetImpl~ "derived lineage"
  createSubsetFromSelection() Dataset~DatasetImpl~ "full lineage"
 }

 class AbstractDataHierarchyManager {

 }
}

  class MyDataFactory  {

  }
  MyDataFactory --() PluginFactory
  MyDataFactory --() RawDataFactory

 class MyData {
  _data : ~D~
  createDataSet() Dataset~DatasetImpl~ 
 }

  MyData --() RawData

 class MyDataset {

 }

  MyDataset --() DatasetImpl

namespace mv {
 class DataHierarchyItem {
 }
}

namespace mv {
  class Dataset~T~ {

  }
}


 AbstractDataManager "1"--> "*" MyDataset : creates<br/> destroys<br/> owns </br> using MyDataFactory 
 AbstractDataHierarchyManager "1" --o "many" DataHierarchyItem : manages
 DataHierarchyItem "1" --o "1" MyDataset : positions in data hierarchy
 MyData --> MyDataset : creates
  MyData --> Dataset : returns
 Dataset~T~ "1" --> "1" MyDataset : access by **guid**

click AbstractDataManager href "https://manivault.readthedocs.io/en/latest/development/building_plugins/data/mental_model.html#three-independent-relationships" "Data set relationships"

click AbstractDataHierarchyManager href "https://manivault.readthedocs.io/en/latest/api/core/managers/abstract_data_hierarchy_manager.html" "API"

click MyData href "https://manivault.readthedocs.io/en/latest/api/core/data/raw_data.html" "API"

click DataHierarchyItem href "https://manivault.readthedocs.io/en/latest/api/core/data/data_hierarchy_item.html" "API"

click Dataset href "https://manivault.readthedocs.io/en/latest/api/core/data/dataset.html" "API"

click MyDataset href "https://manivault.readthedocs.io/en/latest/api/core/data/dataset_impl.html" "API"
```
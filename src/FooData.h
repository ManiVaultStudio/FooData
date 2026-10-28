#pragma once

#include "FooDataPlugin_export.h"

#include "RawData.h"
#include "Set.h"

#include <QString>

#include <vector>
#include <complex>

using namespace mv::plugin;


const mv::DataType FooType = mv::DataType(QString("Foo"));

// =============================================================================
// Raw Data - holds a complex (magniture/phase) image as an array of 
// std::complex<float> values. 
// =============================================================================

class FOODATAPLUGIN_EXPORT FooData : public RawData {
public:
  FooData(const PluginFactory* factory) : RawData(factory, FooType) {}
  ~FooData(void) override;

  void init() override;

  /**
   * Create dataset for raw data
   * @param guid Globally unique dataset identifier (use only for deserialization)
   * @return Smart pointer to dataset
   */
  mv::Dataset<mv::DatasetImpl> createDataSet(const QString& guid = "") const override;

  std::vector<std::complex<float>>& getData() {return _data; };
  const std::vector<std::complex<float>>& getData() const { return _data; }

  void setData(QImage image, QImage phase);
  void setData(std::vector<std::complex<float>> data) { _data = data; }
  size_t getNumPoints() const { return _data.size(); }

private:
  std::vector<std::complex<float>> _data;       /** Data store of the plugin, this is the core of your plugin */
};

// =============================================================================
// Data Set
// Derive the implementation from DatasetImpl. 
// At minimum, implement the abstract data and selection contrac
// =============================================================================

class FOODATAPLUGIN_EXPORT Foo : public mv::DatasetImpl {
public:
  Foo(QString dataName, const QString& guid = "");
  ~Foo() override;

  /**
   * Create subset and specify where the subset will be placed in the data hierarchy
   * @param guiName Name of the subset in the GUI
   * @param parentDataSet Smart pointer to parent dataset in the data hierarchy (default is below the set)
   * @param visible Whether the subset will be visible in the UI
   * @return Smart pointer to the created subset
   */
  mv::Dataset<mv::DatasetImpl> createSubsetFromSelection(const QString& guiName, const mv::Dataset<mv::DatasetImpl>& parentDataSet = mv::Dataset<mv::DatasetImpl>(), const bool& visible = true) const override;

  /** Mandatory override for copying of data sets */
  mv::Dataset<mv::DatasetImpl> copy() const override;

public: // Selection

  /**
   * Get selection indices
   * @return Selection indices
   */
  std::vector<std::uint32_t>& getSelectionIndices() override;

  /**
   * Select by indices
   * @param indices Selection indices
   */
  void setSelectionIndices(const std::vector<std::uint32_t>& indices) override;

  /** Determines whether items can be selected */
  bool canSelect() const override;

  /** Determines whether all items can be selected */
  bool canSelectAll() const override;

  /** Determines whether there are any items which can be deselected */
  bool canSelectNone() const override;

  /** Determines whether the item selection can be inverted (more than one) */
  bool canSelectInvert() const override;

  /** Select all items */
  void selectAll() override;

  /** Deselect all items */
  void selectNone() override;

  /** Invert item selection */
  void selectInvert() override;

  void setData(std::vector<std::complex<float>> data);
  size_t getNumPoints() const;

protected:
  std::vector<unsigned int>   _indices;      /** Indices into the raw data, if this dataset is just a subset */
};

// =============================================================================
// Factory
// =============================================================================

class FooDataPluginFactory : public RawDataFactory {
  Q_INTERFACES(mv::plugin::RawDataFactory mv::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "studio.manivault.FooData"
      FILE  "PluginInfo.json")

public:
  FooDataPluginFactory();

  mv::plugin::RawData* produce() override;
};

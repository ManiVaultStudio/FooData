#pragma once

#include "FooDataPlugin_export.h"

#include "RawData.h"
#include "Set.h"

#include <QString>

#include <vector>
#include <complex>

using namespace mv::plugin;


QString getExampleNotificationMessage() {
  return { "ManiVault Studio features a notification system that can be triggered from:<br>"
      "<table>"
      "<tr>"
      "<th style='text-align: left;'>Within a plugin:  </th>"
      "<td><a href='https://github.com/ManiVaultStudio/core/blob/3088ad09de6b5a5f3e84b24ffba44294bbb8001d/ManiVault/src/Plugin.h#L170'>addNotification(...)</a></td>"
      "</tr>"
      "<tr>"
      "<th style='text-align: left;'>The help manager:  </th>"
      "<td><a href='https://github.com/ManiVaultStudio/core/blob/126bd905d64d9b795dce188a06e9321eef132bd3/ManiVault/src/AbstractHelpManager.h#L100'>mv::help().addNotification(...)</a></td>"
      "</tr>"
      "</table>" };
}

const mv::DataType ExampleType = mv::DataType(QString("Example"));

// =============================================================================
// Raw Data
// =============================================================================

class EXAMPLEDATAPLUGIN_EXPORT FooDataPlugin : public RawData {
public:
  FooDataPlugin(const PluginFactory* factory) : RawData(factory, ExampleType) {}
  ~FooDataPlugin(void) override;

  void init() override;

  /**
   * Create dataset for raw data
   * @param guid Globally unique dataset identifier (use only for deserialization)
   * @return Smart pointer to dataset
   */
  mv::Dataset<mv::DatasetImpl> createDataSet(const QString& guid = "") const override;

  std::vector<std::complex<float>>& getData();

  void setData(QImage image);

private:
  std::vector<std::complex<float>> _data;       /** Data store of the plugin, this is the core of your plugin */
};

// =============================================================================
// Data Set
// =============================================================================

class EXAMPLEDATAPLUGIN_EXPORT ComplexSet : public mv::DatasetImpl {
public:
  ComplexSet(QString dataName, const QString& guid = "");
  ~ComplexSet() override;

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

protected:
  std::vector<unsigned int>   _indices;      /** Indices into the raw data, if this dataset is just a subset */
};

// =============================================================================
// Factory
// =============================================================================

class FooDataPluginFactory : public RawDataFactory {
  Q_INTERFACES(mv::plugin::RawDataFactory mv::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "studio.manivault.FooDataPlugin"
      FILE  "PluginInfo.json")

public:
  FooDataPluginFactory();

  mv::plugin::RawData* produce() override;
};

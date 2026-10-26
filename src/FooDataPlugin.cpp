#include "FooDataPlugin.h"

#include "Application.h"

#include <QtCore>

#include <set>
#include <cassert>

Q_PLUGIN_METADATA(IID "studio.manivault.FooDataPlugin")

using namespace mv;

FooDataPlugin::~FooDataPlugin(void)
{
    
}

void FooDataPlugin::init()
{
    addNotification(getExampleNotificationMessage());
}

/**
 * Create a new dataset linking back to the original raw data
 */
Dataset<DatasetImpl> FooDataPlugin::createDataSet(const QString& guid /*= ""*/) const
{
    return new ComplexSet(getName(), guid);
}

std::vector<std::complex<float>>& FooDataPlugin::getData()
{
    return _data;
}

/**
 * Convenience function for setting data in the plugin from an
 * two images one representing magnitude (real) and the other phase (imaginary).
 * e.g. MRI images.
 */
void FooDataPlugin::setData(QImage image, QImage phase) {
  assert(image.size() == phase.size());
  for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            auto cpixel = std::complex<float>(image.pixel(x, y), phase.pixel(x, y));
            _data.push_back(cpixel);
        }
    }
}

ComplexSet::ComplexSet(QString dataName, const QString& guid /*= ""*/) :
    DatasetImpl(dataName, true, guid)
{
}

ComplexSet::~ComplexSet()
{
}

Dataset<DatasetImpl> ComplexSet::createSubsetFromSelection(const QString& guiName, const Dataset<DatasetImpl>& parentDataSet /*= Dataset<DatasetImpl>()*/, const bool& visible /*= true*/) const
{
    return mv::data().createSubsetFromSelection(getSelection<ComplexSet>(), const_cast<ComplexSet*>(this), guiName, parentDataSet, visible);
}

Dataset<DatasetImpl> ComplexSet::copy() const
{
    auto copySet = new ComplexSet(getRawDataName());

    copySet->_indices = _indices;

    return copySet;
}

std::vector<std::uint32_t>& ComplexSet::getSelectionIndices()
{
    return _indices;
}

void ComplexSet::setSelectionIndices(const std::vector<std::uint32_t>& indices)
{
    _indices = indices;
}

bool ComplexSet::canSelect() const
{
    return getRawData<FooDataPlugin>()->getData().size() >= 1;
}

bool ComplexSet::canSelectAll() const
{
    return canSelect() && (getSelectionSize() < getRawData<FooDataPlugin>()->getData().size());
}

bool ComplexSet::canSelectNone() const
{
    return canSelect() && (getSelectionSize() >= 1);
}

bool ComplexSet::canSelectInvert() const
{
    return canSelect();
}

void ComplexSet::selectAll()
{
    // Get reference to selection indices
    auto& selectionIndices = getSelectionIndices();

    // Clear and resize
    selectionIndices.clear();
    selectionIndices.resize(getRawData<FooDataPlugin>()->getData().size());

    // Generate cluster selection indices
    std::iota(selectionIndices.begin(), selectionIndices.end(), 0);

    // Notify others that the selection changed
    events().notifyDatasetDataSelectionChanged(this);
}

void ComplexSet::selectNone()
{
    // Clear selection indices
    getSelectionIndices().clear();

    // Notify others that the selection changed
    events().notifyDatasetDataSelectionChanged(this);
}

void ComplexSet::selectInvert()
{
    // Get reference to selection indices
    auto& selectionIndices = getSelectionIndices();

    // Create set of selected indices
    std::set<std::uint32_t> selectionSet(selectionIndices.begin(), selectionIndices.end());

    // Get number of items
    const auto numberOfItems = getRawData<FooDataPlugin>()->getData().size();

    // Clear and resize
    selectionIndices.clear();
    selectionIndices.reserve(numberOfItems - selectionSet.size());

    // Do the inversion
    for (std::uint32_t i = 0; i < numberOfItems; i++) {
        if (selectionSet.find(i) == selectionSet.end())
            selectionIndices.push_back(i);
    }

    // Notify others that the selection changed
    events().notifyDatasetDataSelectionChanged(this);
}

// =============================================================================
// Plugin Factory 
// =============================================================================

FooDataPluginFactory::FooDataPluginFactory()
{
    setIconByName("database");

    getPluginMetadata().setDescription("Foo data plugin");
    getPluginMetadata().setSummary("This example shows how to implement and build a basic data plugin with vcpkg in ManiVault Studio.");
    getPluginMetadata().setCopyrightHolder({ "BioVault (Biomedical Visual Analytics Unit LUMC - TU Delft)" });
    getPluginMetadata().setAuthors({
        { "J. Thijssen", { "Software architect" }, { "LUMC", "TU Delft" } },
        { "T. Kroes", { "Lead software architect" }, { "LUMC" } },
        { "A. Vieth",{ "Plugin developer", "Maintainer" },{ "LUMC", "TU Delft" } },
        { "B. va Lew", { "Cat herder", "Maintainer" }, { "LUMC" } }
      });
    getPluginMetadata().setOrganizations({
        { "LUMC", "Leiden University Medical Center", "https://www.lumc.nl/en/" },
        { "TU Delft", "Delft university of technology", "https://www.tudelft.nl/" }
	});
    getPluginMetadata().setLicenseText("This plugin is distributed under the [LGPL v3.0](https://www.gnu.org/licenses/lgpl-3.0.en.html) license.");
}

plugin::RawData* FooDataPluginFactory::produce()
{
    return new FooDataPlugin(this);
}
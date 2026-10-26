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
    return new PixelSet(getName(), guid);
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
            cpixel = std::complex<float>(image.pixel(x, y), phase.pixel(x, y));
            _data.push_back(cpixel);
        }
    }
}
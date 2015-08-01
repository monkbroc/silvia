// Set and get storage values through the cloud

#include "cloud_storage.h"
#include "helpers.h"

const CloudStorage::CloudStorageMethod CloudStorage::cloudStorageMethods[] = {
  { "sp", &Storage::setTargetTemperature, &Storage::getTargetTemperature },
  { "Kp", &Storage::setKp, &Storage::getKp },
  { "Ki", &Storage::setKi, &Storage::getKi },
  { "Ko", &Storage::setKo, &Storage::getKo },
  { "iSat", &Storage::setiTermSaturation, &Storage::getiTermSaturation },
  { "iBand", &Storage::setIntegralErrorBand, &Storage::getIntegralErrorBand },
  { "sleep", &Storage::setSleep, &Storage::getSleep },
  { "Twakeup", &Storage::setTwakeup, &Storage::getTwakeup },
};

// Singleton instance
CloudStorage cloudStorage;

// Spark.function and Spark.variable expect globals.
// Static at least restricts them to this file.
static int cloudSet(String value) {
  return cloudStorage.set(value);
}
static int cloudGet(String value) {
  return cloudStorage.get(value);
}
static char getResult[64];

void CloudStorage::setup(Storage *storage) {
  this->storage = storage;

  Spark.function("set", cloudSet);
  Spark.function("get", cloudGet);
  Spark.variable("result", getResult, STRING);
}

int CloudStorage::set(String assignment) {
  int equalSign = assignment.indexOf("=");
  if(equalSign >= 0) {
    String name = assignment.substring(0, equalSign);
    String value = assignment.substring(equalSign + 1);

    const CloudStorageMethod *csm = csmByName(name);
    if(csm) {
      if(csm->setDouble) {
        // Call method on storage object
        (storage->*(csm->setDouble))(value.toFloat());
        return 0;
      } else {
        return -3; /* Nothing defined?!? */
      }
    } else {
      return -2; /* Variable name not found */
    }
  } else {
    return -1; /* Wrong format */
  }
}

int CloudStorage::get(String name) {
  const CloudStorageMethod *csm = csmByName(name);
  if(csm) {
    if(csm->getDouble) {
      // Call method on storage object
      double value = (storage->*(csm->getDouble))();
      int precision;
      if(value > 1000000) {
        precision = 0;
      } else if(value > 1000) {
        precision = 3;
      } else {
        precision = 6;
      }
      setResult(String(value, precision));
      return 0;
    } else {
      return -3; /* Nothing defined?!? */
    }
  } else {
    return -2; /* Variable name not found */
  }
}

const CloudStorage::CloudStorageMethod *CloudStorage::csmByName(String name) {
  for(unsigned int i = 0; i < countof(cloudStorageMethods); i++) {
    const CloudStorageMethod *csm = &cloudStorageMethods[i];
    if(name.equals(csm->name)) {
      return csm;
    }
  }
  return NULL;
}

void CloudStorage::setResult(String value) {
  // Set global char array
  value.toCharArray(getResult, countof(getResult));
}

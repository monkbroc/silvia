// Set and get storage values through the cloud

#ifndef __CLOUD_STORAGE_H__
#define __CLOUD_STORAGE_H__

#include "application.h"
#include "storage.h"

class CloudStorage {
  public:
  void setup(Storage *storage);

  // Should only be called by cloudSet and cloudGet
  int set(String assignment);
  int get(String name);

  private:

  void setResult(String value);

  typedef void(Storage::*SetDoubleMethod)(double);
  typedef double(Storage::*GetDoubleMethod)();

  struct CloudStorageMethod {
    String name;
    SetDoubleMethod setDouble;
    GetDoubleMethod getDouble;
  };

  static const CloudStorageMethod cloudStorageMethods[];

  const CloudStorageMethod *csmByName(String name);

  Storage *storage;
};

// Singleton instance
extern CloudStorage cloudStorage;

#endif // __CLOUD_STORAGE_H__

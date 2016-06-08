// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4VPersistentSubDbMan.hh,v 1.3 1999/11/28 21:54:19 morita Exp $
// GEANT4 tag $Name: geant4-01-00 $
//
// class G4VPersistentSubDbMan 
//
// Abstract class to be used by G4PersistencyManager
// and G4TransactionManager
//
// Member functions:
// =================
//  void SetDB(HepDatabaseRef aDB);
//    Set DB reference.
//  HepDatabaseRef GetDB();
//    Get DB reference.
//  void SetContainer(HepContainerRef container);
//    Set Container reference.
//  HepContainerRef GetContainer();
//    Get Container reference.
//
//  Note: inherited concrete class should implement Store() and Retrieve()
//        and also should set f_currentContainer in Retieve()
//    Store(HepDbApplication* dbApp, const object_type* object);
//    Retrieve(HepDbApplication* dbApp, object_type*& object);
//
// Member data:
// ============
//  HepContainerRef f_DB;
//    Reference to the database.
//  HepContainerRef f_container;
//    Reference to the container in the database.
//  HepContainerRef f_currentContainer;
//    Reference to the current scan scope in Retrieve().
//
//    example in a piece of Retrive():
//      if( f_container != f_currentContainer )
//      {
//        f_currentContainer = f_container;
//        pRun_iterator.scan(f_currentContainer);
//      }
//
// History:
// 99.11.25 Y.Morita  Initial creation

#ifndef G4VPersistentSubDbMan_hh
#define G4VPersistentSubDbMan_hh 1

#include "globals.hh"
#include "HepODBMS/odbms/HepODBMS.h"

#include "G4PersistentTypes.hh"
#include "G4DatabaseTypes.hh"

class G4VPersistentSubDbMan 
{
  friend class G4PersistencyManager;
  friend class G4TransactionManager;

  protected:
      // to be constructed by G4PersistencyManager only
      G4VPersistentSubDbMan();
      virtual ~G4VPersistentSubDbMan();

  protected:
      // interface with G4PersistencyManager
      // Store(HepDbApplication* dbApp, const object_type* object);
      // Retrieve(HepDbApplication* dbApp, object_type*& object);

  protected:
      // interface with G4TransactionManager
      inline void SetDB(HepDatabaseRef aDB)
      { f_DB = aDB; }
      inline void SetContainer(HepContainerRef container)
      { f_container = container; }

  public:
      inline HepDatabaseRef GetDB()
      { return f_DB; }
      inline HepContainerRef GetContainer()
      { return f_container; }

  protected:
      HepDatabaseRef  f_DB;
      HepContainerRef f_container;
      HepContainerRef f_currentContainer;
};

#endif

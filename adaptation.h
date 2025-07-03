#ifndef ADAPTATION_H
#define ADAPTATION_H

#if defined(__unix__) || defined(__APPLE__)
    // On Unix-like

    // win zone
    #include "dialogyear.h"
    #include "dialoggroup.h"
    #include "dialogteacher.h"
    #include "dialogstudent.h"
    #include "dialogchosegroup.h"
    #include "dialogchoseteacher.h"
    #include "dialogchosecomis.h"
    #include "dialogsearch.h"

    // module zone
    #include "databasemanager.h"
    #include "fconfigmanager.h"
    #include "docxmanager.h"

#elif defined(_WIN32) || defined(_WIN64)
    // On WindowsNT
#endif

#endif // ADAPTATION_H

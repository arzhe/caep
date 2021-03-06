/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : noncopyable.h                                                *
 *                                                                                             *
 *                   Programmer : Guan Zhe                                                     *
 *                                                                                             *
 *                   Start Date : Aug 22, 2019                                                 *
 *                                                                                             *
 *                  Last Update : Dec 1, 2021   [ArZr]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   noncopyable::noncopyable -- Constructor for noncopyable.                                  *
 *   noncopyable::~noncopyable -- Destructor for noncopyable.                                  *
 *   noncopyable::noncopyable -- Prohibits copy.                                               *
 *   noncopyable::operator= -- Prohibits assignment.                                           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CAEP_NONCOPYABLE_H
#define CAEP_NONCOPYABLE_H

namespace caep {


/*
 * @breif Class noncopyable is provided for the class who wanna prohibit
 * copy and assigment, but constructor and destructor continue to function normally.
 */
class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

}
#endif

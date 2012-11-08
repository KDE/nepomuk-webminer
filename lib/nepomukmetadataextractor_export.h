/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NEPOMUKMETADATAEXTRACTOR_EXPORT_H
#define NEPOMUKMETADATAEXTRACTOR_EXPORT_H

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>

#ifndef NEPOMUKMETADATAEXTRACTOR_EXPORT
# if defined(MAKE_NEPOMUKMETADATAEXTRACTOR_LIB)
// We are building this library
#  define NEPOMUKMETADATAEXTRACTOR_EXPORT KDE_EXPORT
# else
// We are using this library
#  define NEPOMUKMETADATAEXTRACTOR_EXPORT KDE_IMPORT
# endif
#endif

# ifndef NEPOMUKMETADATAEXTRACTOR_EXPORT_DEPRECATED
#  define NEPOMUKMETADATAEXTRACTOR_EXPORT_DEPRECATED KDE_DEPRECATED NEPOMUKMETADATAEXTRACTOR_EXPORT
# endif

#endif

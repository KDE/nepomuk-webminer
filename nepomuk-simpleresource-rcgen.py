#!/usr/bin/env python

## This file is part of the Nepomuk KDE project.
## Copyright (C) 2011 Sebastian Trueg <trueg@kde.org>
## Copyright (C) 2011 Serebriyskiy Artem <v.for.vandal@gmail.com>
## Copyright (C) 2013 Joerg Ehrichs <joerg.ehrichs@gmx.de>
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) version 3, or any
## later version accepted by the membership of KDE e.V. (or its
## successor approved by the membership of KDE e.V.), which shall
## act as a proxy defined in Section 6 of version 3 of the license.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this library.  If not, see <http://www.gnu.org/licenses/>.


import argparse
import sys
import os, errno
from PyKDE4.soprano import Soprano
from PyQt4 import QtCore
import pprint

# A list of C++ reserved keywords which we need to handle
cppKeywords = ['class', 'int', 'float', 'double', 'long']

# global variables for speedy typeString (please change comment to something more serious! :)
rdfTypeQStringList = {Soprano.Vocabulary.XMLSchema.string().toString(): 'QStringList',
                      Soprano.Vocabulary.RDFS.Literal().toString(): 'QStringList'}
                      
rdfTypeOthers = {Soprano.Vocabulary.XMLSchema.integer().toString(): "qint64",
                 Soprano.Vocabulary.XMLSchema.negativeInteger().toString(): "qint64",
                 Soprano.Vocabulary.XMLSchema.nonNegativeInteger().toString(): "quint64",
                 Soprano.Vocabulary.XMLSchema.xsdLong().toString(): "qint64",
                 Soprano.Vocabulary.XMLSchema.unsignedLong().toString(): "quint64",
                 Soprano.Vocabulary.XMLSchema.xsdInt().toString(): "qint32",
                 Soprano.Vocabulary.XMLSchema.unsignedInt().toString(): "quint32",
                 Soprano.Vocabulary.XMLSchema.xsdShort().toString(): "qint16",
                 Soprano.Vocabulary.XMLSchema.unsignedShort().toString(): "quint16",
                 Soprano.Vocabulary.XMLSchema.xsdFloat().toString(): "double",
                 Soprano.Vocabulary.XMLSchema.xsdDouble().toString(): "double",
                 Soprano.Vocabulary.XMLSchema.boolean().toString(): "bool",
                 Soprano.Vocabulary.XMLSchema.date().toString(): "QDate",
                 Soprano.Vocabulary.XMLSchema.time().toString(): "QTime",
                 Soprano.Vocabulary.XMLSchema.dateTime().toString(): "QDateTime",
                 Soprano.Vocabulary.XMLSchema.string().toString(): "QString",
                 Soprano.Vocabulary.RDFS.Literal().toString(): "QString"}


output_path = os.getcwd()
verbose = True


class OntologyParser():
    def __init__(self):
        self.parsedData = {}
        self.classes = {}
        self.ontologies = {}
        self.specialProperties = []

    '''
    @brief Read the trig file for the ontology at 'path' via the Soprano parser and save the content in a dictionary
    
    @return @arg @c true if reading was sucessfull
            @arg @c false if file could not be parsed
    '''
    def parseFile(self, path):
        parser = Soprano.PluginManager.instance().discoverParserForSerialization(Soprano.SerializationTrig)
        if not parser:
            return False
            
        it = parser.parseFile(path, QtCore.QUrl("dummy"), Soprano.SerializationTrig)
        while it.next():
            # get existing dictionay for the current subject
            subject = {}
            if it.current().subject().toString() in self.parsedData:
                subject = self.parsedData[it.current().subject().toString()]
            
            # now add the new context
            
            # special case for schema#subClassOf as we can have more than 1 of them
            if it.current().predicate().toString() == Soprano.Vocabulary.RDFS.subClassOf().toString():
                parentClassList = []
                
                if Soprano.Vocabulary.RDFS.subClassOf().toString() in subject:
                    parentClassList = subject[Soprano.Vocabulary.RDFS.subClassOf().toString()]
                    
                parentClassList.append(it.current().object().toString())
                subject[Soprano.Vocabulary.RDFS.subClassOf().toString()] = parentClassList
            
            # for all other predicates we simply add them
            else:
                subject[it.current().predicate().toString()] = it.current().object().toString()
                
            subject[u'context'] = it.current().context().toString()

            self.parsedData.update({it.current().subject().toString(): subject})

        if parser.lastError():
            return False;

        return True
    
    '''
    @brief Sort the parsed ontology dictionary
    
    split data so we end up with
    * one dictionary per class with all properties. combined in self.classes
    * one dictionary for each general ontology data. combined in self.ontologies
    '''
    def sortData(self):
        # iterate of all parsed entries
        for key in self.parsedData: 
            # check what kind of entry we have here
            #print 'check key: ' + key + ' for typeKey: ' + typeKey
            
            if Soprano.Vocabulary.RDF.type().toString() not in self.parsedData[key]:
                continue
            
            # for ontologies simply throw them in the ontology dictionary
            elif self.parsedData[key][Soprano.Vocabulary.RDF.type().toString()] == Soprano.Vocabulary.NRL.Ontology().toString():
                self.ontologies[key] = self.parsedData[key]
            
            # for classes find all properties and throw them in the class dictionary
            elif self.parsedData[key][Soprano.Vocabulary.RDF.type().toString()] == Soprano.Vocabulary.RDFS.Class().toString():                
                self.classes[key] = self.buildClassDictionary(key, self.parsedData[key])
            
            
            # anything else can be ignored (properties and enums)
            
        #FIXME: add some special cases for all properties with a domain of rdfs:Resource and no domain
        #       these properties will be added to all classes with no parent class other than Nepomuk2::SimpleResource
        #       otherwise properties like nao:prefLabelel are not working
        #NOTE:  These properties might have the same names as other properties (nco:nco#contributor/nao:nco#contributor or nie:description/nao:description)
        #       So we add the namespace in front of them to be sure (and keep build compatibility with old nepomuk-simpleresource-rcgen)
        #       This is not done automatically on purpose, so we even add the namespace if we do not use all conflictiong ontologies
        #       this way the generated classes always look the same.
        #       @see extractNameFromUri
        for key in self.parsedData: 
            if Soprano.Vocabulary.RDF.type().toString() not in self.parsedData[key]:
                continue
            
            if self.parsedData[key][Soprano.Vocabulary.RDF.type().toString()] != Soprano.Vocabulary.RDF.Property().toString():
                continue
            
            if Soprano.Vocabulary.RDFS.domain().toString() not in self.parsedData[key] or self.parsedData[key][Soprano.Vocabulary.RDFS.domain().toString()] == Soprano.Vocabulary.RDFS.Resource().toString():
                specialDict =  self.parsedData[key]
                specialDict[u'uri'] = key
                self.specialProperties.append(specialDict)
    
    '''
    @brief Takes the dictionary that is used for the class and creates a new dictionary with all properties
    
    In order to do this, all entries that have rdf-schema#domain with this class are taken and combined
    
    @return a new dictionary with all data necessary for this class construct
    '''
    def buildClassDictionary(self, className, classData):
        
        classDict = {}
        propertyList = []
        
        # first get the general information for the class
        if u'context' in classData:
            classDict[u'context'] = classData[u'context'] # what ontology it belongs to
        if Soprano.Vocabulary.RDFS.subClassOf().toString() in classData:
            classDict[u'parent'] = classData[Soprano.Vocabulary.RDFS.subClassOf().toString()] # the parent class if it exist
        if Soprano.Vocabulary.RDFS.comment().toString() in classData:
            classDict[u'comment'] = classData[Soprano.Vocabulary.RDFS.comment().toString()] # the class comment
        if Soprano.Vocabulary.RDFS.label().toString() in classData:
            classDict[u'label'] = classData[Soprano.Vocabulary.RDFS.label().toString()] # the class name
            
        #---------------------
        # Now get all properties of this class from the parsedData dictionary
        for key in self.parsedData: 
            # check if it is a property in the domain of this class
            if Soprano.Vocabulary.RDFS.domain().toString() not in self.parsedData[key]:
                continue
            
            if self.parsedData[key][Soprano.Vocabulary.RDFS.domain().toString()] != className:
                continue
            
            # ok we have a property of this class put it into the list of all properties
            propDict = self.parsedData[key]
            propDict[u'uri'] = key
            propertyList.append(propDict)
            
        # now add properties to class dictionary and return the class
        classDict[u'properties'] = propertyList
         
        return classDict
    
    '''
    @brief iterates over all classes and generates the header files
    '''
    def generateClasses(self):
        for key in self.classes:
            self.writeHeader(key, self.classes[key])
    
    '''
    @brief Normalize a class or property name to be used as a C++ entity.
    '''
    def normalizeName(self, name):
        name.replace('-', '_')
        name.replace('.', '_')
        return name
    
    '''
    @brief Extract the class or property name from an entity URI. This is the last section of the URI
    '''
    def extractNameFromUri(self, uri):
        #NOTE: Here some special cases are handled, cases where the property name are the same for different ontologies
        #      In order to keep the same method names, no matter if all pntologies are used we hardcode them here
        #      usually this should not happend to more than a handful cases and rather be avoided during ontology creation anyway
        if uri == u'http://www.semanticdesktop.org/ontologies/2007/03/22/nco#contributor':
            name = QtCore.QString('ncoContributor')
        elif uri == u'http://www.semanticdesktop.org/ontologies/2007/03/22/nao#contributor':
            name = QtCore.QString('naoContributor')
        elif uri == u'http://www.semanticdesktop.org/ontologies/2007/08/15/nao#description':
            name = QtCore.QString('naoDescription')
        elif uri == u'http://www.semanticdesktop.org/ontologies/2007/01/19/nie#description':
            name = QtCore.QString('nieDescription')
        elif uri == u'http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator':
            name = QtCore.QString('naoCreator')
        elif uri == u'http://www.semanticdesktop.org/ontologies/2007/08/15/nco#creator':
            name = QtCore.QString('ncoCreator')
        elif uri == u'http://www.semanticdesktop.org/ontologies/2007/01/19/nie#modified':
            name = QtCore.QString('nieModified')
        elif uri == u'http://www.semanticdesktop.org/ontologies/2007/08/15/nao#modified':
            name = QtCore.QString('naoModified')
        elif uri == u'http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created':
            name = QtCore.QString('naoCreated')
        elif uri == u'http://www.semanticdesktop.org/ontologies/2007/04/02/ncal#created':
            name = QtCore.QString('ncalCreated')
        elif uri == u'http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified':
            name = QtCore.QString('naoLastModified')
        elif uri == u'http://www.semanticdesktop.org/ontologies/2007/01/19/nie#lastModified':
            name = QtCore.QString('nieLastModified')
        else:
            name = uri.mid(uri.lastIndexOf(QtCore.QRegExp('[#/:]'))+1)
        return self.normalizeName(name)
    
    '''
    @brief Create a folder and all its missing parent folders
    '''
    def mkdir_p(self, path):
        try:
            os.makedirs(path)
        except OSError as exc: # Python >2.5
            if exc.errno == errno.EEXIST:
                pass
            else: raise

    '''
    @brief Construct the C++/Qt type to be used for the given type and cardinality.
           Uses QUrl for all non-literal types
    '''
    def typeString(self, rdfType, cardinality):
        if (rdfType in rdfTypeQStringList) and cardinality != 1:
            return 'QStringList'

        simpleType = rdfTypeOthers.get(rdfType, 'QUrl') # fall back to QUrl when not matched
    
        if cardinality != 1:
            return 'QList<%s>' % simpleType
        else:
            return simpleType

    '''
    @brief adds some additional adjectives to class getter/setter names 
    '''
    def makeFancy(self, name, cardinality):
        if name.startsWith("has"):
            name = name[3].toLower() + name.mid(4)
        if cardinality != 1:
            if name.endsWith('s'):
                name = name + 'es'
            else:
                name = name + 's'

        return self.normalizeName(name)
    
    '''
    @brief Extracts some class generic details
    
    @return list with className and nameSpace Abbriviation
    '''
    def classDetails(self, classUri):
        className = self.extractNameFromUri(classUri)
        nsAbbr = None
        classDetails = []
        
        if classUri not in self.classes:
            return classDetails
    
        # find the default namespace abbreviation, usualy this is inside the ontology.trig file
        ontology = self.classes[classUri][u'context']
        if ontology in self.ontologies and Soprano.Vocabulary.NAO.hasDefaultNamespaceAbbreviation().toString() in self.ontologies[ontology]:
            nsAbbr = self.ontologies[ontology][Soprano.Vocabulary.NAO.hasDefaultNamespaceAbbreviation().toString()]
        else:
            #if not we parse it out of the uri
            nsAbbr = ontology.mid(ontology.lastIndexOf(QtCore.QRegExp('[/]'))+1, -1).remove('#')

        if nsAbbr is None:
            print 'Could not find NameSpace Abbreviation from ' + ontology
        
        classDetails = [className, nsAbbr]
        
        return classDetails;
    
    '''
    @brief writes a comment for a class/getter/setter/adder methods
    '''
    def writeComment(self, theFile, text, indent):
        maxLine = 50;

        theFile.write(' ' * indent*4)
        theFile.write("/**\n")
        theFile.write(' ' * (indent*4+1))
        theFile.write("* ")

        words = QtCore.QString(text).split( QtCore.QRegExp("\\s"), QtCore.QString.SkipEmptyParts )
        cnt = 0;
        for i in range(words.count()):
            if cnt >= maxLine:
                theFile.write('\n')
                theFile.write(' ' * (indent*4+1))
                theFile.write("* ")
                cnt = 0;
            theFile.write(words[i])
            theFile.write(' ')
            cnt += words[i].length()

        theFile.write('\n')
        theFile.write(' ' * (indent*4+1))
        theFile.write("*/\n")

    '''
    @brief Writes the getter method for any given ontology class property
    '''
    def writeGetter(self, theFile, propUri, name, propRange, cardinality):
        fancyName = self.makeFancy(name, cardinality)
        if fancyName in cppKeywords:
            fancyName = 'get' + fancyName[0].toUpper() + fancyName.mid(1)
            
        theFile.write('    %s %s() const {\n' % (self.typeString(propRange, cardinality), fancyName))
        theFile.write('        %s value;\n' % self.typeString(propRange, cardinality))
        if cardinality == 1:
            theFile.write('        if(contains(QUrl::fromEncoded("%s", QUrl::StrictMode)))\n' % propUri)
            theFile.write('            value = property(QUrl::fromEncoded("{0}", QUrl::StrictMode)).first().value<{1}>();\n'.format(propUri, self.typeString(propRange, 1)))
        else:
            theFile.write('        foreach(const QVariant& v, property(QUrl::fromEncoded("%s", QUrl::StrictMode)))\n' % propUri)
            theFile.write('            value << v.value<{0}>();\n'.format(self.typeString(propRange, 1)))
        theFile.write('        return value;\n')
        theFile.write('    }\n')
        

    def writeSetter(self, theFile, propUri, name, propRange, cardinality):
        theFile.write('    void set%s%s(const %s& value) {\n' % (self.makeFancy(name, cardinality)[0].toUpper(), self.makeFancy(name, cardinality).mid(1), self.typeString(propRange, cardinality)))
        theFile.write('        QVariantList values;\n')
        if cardinality == 1:
            theFile.write('        values << value;\n')
        else:
             theFile.write('        foreach(const %s& v, value)\n' % self.typeString(propRange, 1))
             theFile.write('            values << v;\n')
        theFile.write('        setProperty(QUrl::fromEncoded("%s", QUrl::StrictMode), values);\n' % propUri)
        theFile.write('    }\n')

    def writeAdder(self, theFile, propUri, name, propRange, cardinality):
        theFile.write('    void add%s%s(const %s& value) {\n' % (self.makeFancy(name, 1)[0].toUpper(), self.makeFancy(name, 1).mid(1), self.typeString(propRange, 1)))
        theFile.write('        addProperty(QUrl::fromEncoded("%s", QUrl::StrictMode), value);\n' % propUri)
        theFile.write('    }\n')
        
    '''
    @brief Writes the header file for the current class
    '''
    def writeHeader(self, classUri, classDict):
        
        classInfo = self.classDetails(classUri)
        className = classInfo[0]
        nsAbbr = classInfo[1]
        
        # Construct paths
        relative_path = nsAbbr + '/' + className.toLower() + '.h'
        folder = output_path + '/' + nsAbbr
        filePath = output_path + '/' + relative_path

        print "Writing header file: " + filePath

        # Create the containing folder
        self.mkdir_p(QtCore.QFile.encodeName(folder).data())

        # open the header file
        header = open(filePath, 'w')

        # write protecting ifdefs
        header_protect = '_%s_%s_H_' % (nsAbbr.toUpper(), className.toUpper())
        header.write('#ifndef %s\n' % header_protect)
        header.write('#define %s\n' % header_protect)
        header.write('\n')
        
        # write default includes
        header.write('#include <QtCore/QVariant>\n')
        header.write('#include <QtCore/QStringList>\n')
        header.write('#include <QtCore/QUrl>\n')
        header.write('#include <QtCore/QDate>\n')
        header.write('#include <QtCore/QTime>\n')
        header.write('#include <QtCore/QDateTime>\n')
        header.write('\n')

        # all classes need the SimpleResource include
        header.write('#include <Nepomuk2/SimpleResource>\n\n')

        # get all base classes which we require due to the virtual base class constructor ordering in C++
        parentClassNames = []
        if u'parent' in classDict:
            for parent in classDict[u'parent']:
                if not parent.contains(u'http://www.w3.org/2000/01/rdf-schema#Resource'):
                    parentInfo = self.classDetails(parent)
                    if len(parentInfo) > 0:
                        header.write('#include "%s/%s.h"\n' % (parentInfo[1], parentInfo[0].toLower()))
                        parentClassNames.append("%s::%s" %(parentInfo[1].toUpper(), parentInfo[0]))
            
        header.write('\n')
        
        # We inverse the order to match the virtual inheritance constructor calling order
        parentClassNames.reverse()
            
        # write the class namespace
        header.write('namespace Nepomuk2 {\n')
        header.write('namespace %s {\n' % nsAbbr.toUpper())
        
        # write the header comment
        if u'comment' in classDict:
            self.writeComment(header, classDict[u'comment'], 0)
        
        # write the class + parent classes
        # We use virtual inheritance when deriving from SimpleResource since our ontologies
        # make use of multi-inheritance and without it the compiler would not know which
        # addProperty and friends to call.
        # We need to do the same with all parent classes since some classes like
        # nco:CellPhoneNumber as derived from other classes that have yet another parent
        # class in common which is not SimpleResource.
        header.write('class %s' % className)
        header.write(' : ')
        header.write(', '.join(['public virtual %s' % (p) for p in parentClassNames]))
        if len(parentClassNames) == 0:
            header.write('public virtual Nepomuk2::SimpleResource');
        header.write('\n{\n')
        header.write('public:\n')
        
        
        # write the default constructor
        # We directly set the type of the class to the SimpleResource. If the class is a base class
        # not derived from any other classes then we set the type directly. Otherwise we use the
        # protected constructor defined below which takes a type as parameter making sure that we
        # only add one type instead of the whole hierarchy
        header.write('    %s(const QUrl& uri = QUrl())\n' % className)
        header.write('      : ')
        header.write('SimpleResource(uri)')
        if len(parentClassNames) > 0:
            header.write(', ')
            header.write(', '.join([('%s(uri, QUrl::fromEncoded("' + classUri.toUtf8().data() + '", QUrl::StrictMode))') % p for p in parentClassNames]))
        header.write(' {\n')
        if len(parentClassNames) == 0:
            header.write('        addType(QUrl::fromEncoded("%s", QUrl::StrictMode));\n' % classUri.toUtf8().data())
        header.write('    }\n\n')

        # write the copy constructor
        header.write('    %s(const SimpleResource& res)\n' % className)
        header.write('      : ')
        header.write('SimpleResource(res)')
        if len(parentClassNames) > 0:
            header.write(', ')
            header.write(', '.join([('%s(res, QUrl::fromEncoded("' + classUri.toUtf8().data() + '", QUrl::StrictMode))') % p for p in parentClassNames]))
        header.write(' {\n')
        if len(parentClassNames) == 0:
            header.write('        addType(QUrl::fromEncoded("%s", QUrl::StrictMode));\n' % classUri.toUtf8().data())
        header.write('    }\n\n')

        # write the assignment operator
        header.write('    %s& operator=(const SimpleResource& res) {\n' % className)
        header.write('        SimpleResource::operator=(res);\n')
        header.write('        addType(QUrl::fromEncoded("%s", QUrl::StrictMode));\n' % classUri.toUtf8().data())
        header.write('        return *this;\n')
        header.write('    }\n\n')
        
        
        for p in classDict[u'properties']:
            print 'add property: ' + p[u'uri']
            comment = ''
            if Soprano.Vocabulary.RDFS.comment().toString() in p:
                comment = p[Soprano.Vocabulary.RDFS.comment().toString()]
            
            pcardinality = 0
            if Soprano.Vocabulary.NRL.cardinality().toString() in p:
                pcardinality = int(p[Soprano.Vocabulary.NRL.cardinality().toString()])
            if Soprano.Vocabulary.NRL.maxCardinality().toString() in p:
                pcardinality = int(p[Soprano.Vocabulary.NRL.maxCardinality().toString()])
                
            plabel = p[Soprano.Vocabulary.RDFS.label().toString()]
            pname = self.extractNameFromUri(p[u'uri'])
            
            prange = ' '
            if Soprano.Vocabulary.RDFS.range().toString() in p:
                prange = p[Soprano.Vocabulary.RDFS.range().toString()]
            else:
                print 'No range given for property: ' + pname
                continue
            
            self.writeComment(header, 'Get property %s. %s' % (plabel, comment), 1)
            self.writeGetter(header, p[u'uri'], pname, prange, pcardinality)
            header.write('\n')
            self.writeComment(header, 'Set property %s. %s' % (plabel, comment), 1)
            self.writeSetter(header, p[u'uri'], pname, prange, pcardinality)
            header.write('\n')
            self.writeComment(header, 'Add value to property %s. %s' % (plabel, comment), 1)
            self.writeAdder(header, p[u'uri'], pname, prange, pcardinality)
            header.write('\n')
        
        # Also add all properties with #domain rdfs-Resource or none to any class without parent
        if len(parentClassNames) == 0:
            for sp in self.specialProperties: 
            
                print 'add special propertie for: ' + sp[u'uri']
            
                comment = 'unknown'
                if Soprano.Vocabulary.RDFS.comment().toString() in sp:
                    comment = sp[Soprano.Vocabulary.RDFS.comment().toString()]
                
                pcardinality = 1
                if Soprano.Vocabulary.NRL.cardinality().toString() in sp:
                    pcardinality = int(sp[Soprano.Vocabulary.NRL.cardinality().toString()])
                if Soprano.Vocabulary.NRL.maxCardinality().toString() in sp:
                    pcardinality = int(sp[Soprano.Vocabulary.NRL.maxCardinality().toString()])
                
                plabel = 'unknown'
                if Soprano.Vocabulary.RDFS.label().toString() in sp:
                    plabel = sp[Soprano.Vocabulary.RDFS.label().toString()]
           
                pname = self.extractNameFromUri(sp[u'uri'])
                
                prange = ' '
                if Soprano.Vocabulary.RDFS.range().toString() in sp:
                    prange = sp[Soprano.Vocabulary.RDFS.range().toString()]
                else:
                    print 'No range given for property: ' + sp[u'uri']
                    continue

                self.writeComment(header, 'Get property %s. %s' % (plabel, comment), 1)
                self.writeGetter(header, sp[u'uri'], pname, prange, pcardinality)
                header.write('\n')
                self.writeComment(header, 'Set property %s. %s' % (plabel, comment), 1)
                self.writeSetter(header, sp[u'uri'], pname, prange, pcardinality)
                header.write('\n')
                self.writeComment(header, 'Add value to property %s. %s' % (plabel, comment), 1)
                self.writeAdder(header, sp[u'uri'], pname, prange, pcardinality)
                header.write('\n')
                
        
        # write the protected constructors which avoid adding the whole type hierarchy
        header.write('protected:\n')
        header.write('    %s(const QUrl& uri, const QUrl& type)\n' % className)
        header.write('      : ')
        header.write('SimpleResource(uri)')
        if len(parentClassNames) > 0:
            header.write(', ')
            header.write(', '.join(['%s(uri, type)' % p for p in parentClassNames]))
        header.write(' {\n')
        if len(parentClassNames) == 0:
            header.write('        addType(type);\n')
        header.write('    }\n')

        header.write('    %s(const SimpleResource& res, const QUrl& type)\n' % className)
        header.write('      : ')
        header.write('SimpleResource(res)')
        if len(parentClassNames) > 0:
            header.write(', ')
            header.write(', '.join(['%s(res, type)' % p for p in parentClassNames]))
        header.write(' {\n')
        if len(parentClassNames) == 0:
            header.write('        addType(type);\n')
        header.write('    }\n')
        
        # close the class
        header.write('};\n')

        # write the closing parenthesis for the namespaces
        header.write('}\n}\n')

        # write the closing preprocessor thingi
        header.write('\n#endif\n')
        
        
    def printOntology(self):
        pp = pprint.PrettyPrinter(indent=4)
        pp.pprint(self.ontologies)
        
        print '############ Class overview ############'
        pp.pprint(self.classes)
        
        
        print '############ parsed Data overview ############'
        pp.pprint(self.parsedData)
        
        print '############ specialProperties Data overview ############'
        pp.pprint(self.specialProperties)

def main():
    global output_path
    global verbose
    
    usage = "Usage: %prog [options] ontologyfile1 ontologyfile2 ..."
    optparser = argparse.ArgumentParser(description="Nepomuk SimpleResource code generator. It will generate a hierarchy of simple wrapper classes around Nepomuk2::SimpleResource which provide convinience methods to get and set properties of those classes. Each wrapper class will be defined in its own header file and be written to a subdirectory named as the default ontology prefix. Example: the header file for nao:Tag would be written to nao/tag.h and be defined in the namespace Nepomuk2::NAO.")
    optparser.add_argument('--output', '-o', type=str, nargs=1, metavar='PATH', dest='output', help='The destination folder')
    optparser.add_argument('--quiet', '-q', action="store_false", dest="verbose", default=True, help="don't print status messages to stdout")
    optparser.add_argument("ontologies", type=str, nargs='+', metavar="ONTOLOGY", help="Ontology files to use")

    args = optparser.parse_args()
    if args.output :
        output_path = args.output[0]

    if verbose:
        print 'Generating from ontology files %s' % ','.join(args.ontologies)
        print 'Writing files to %s.' % output_path

    # Parse all ontology files
    ontoParser = OntologyParser()
    for f in args.ontologies:
        if verbose:
            print "Reading ontology '%s'" % f
        ontoParser.parseFile(f)
    if verbose:
        print "All ontologies read. Generating code..."

    # Get all classes and handle them one by one
    #ontoParser.writeAll()
    ontoParser.sortData()
    ontoParser.printOntology()
    
    ontoParser.generateClasses()

















































if __name__ == "__main__":
    main()
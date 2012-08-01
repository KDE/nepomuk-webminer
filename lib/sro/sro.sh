 
#python ./nepomuk-simpleresource-rcgen.py  /usr/share/ontology/nie/nfo.trig /usr/share/ontology/nie/nbib.trig /usr/share/ontology/nie/nco.trig /usr/share/ontology/nie/nfo.trig /usr/share/ontology/nie/nmm.trig /usr/share/ontology/pimo/pimo.trig



#python ./nepomuk-simpleresource-rcgen.py /usr/share/ontology/nepomuk/nao.trig

mkdir nie
mkdir nfo
mkdir nbib
mkdir nco
mkdir nmm
mkdir nao
mkdir pimo

nepomuk2-rcgen --writeall --target ./nie /usr/share/ontology/nie/nie.trig

nepomuk2-rcgen --writeall --target ./nfo /usr/share/ontology/nie/nfo.trig

nepomuk2-rcgen --writeall --target ./nbib /usr/share/ontology/nie/nbib.trig

nepomuk2-rcgen --writeall --target ./nco /usr/share/ontology/nie/nco.trig

nepomuk2-rcgen --writeall --target ./nmm /usr/share/ontology/nie/nmm.trig
nepomuk2-rcgen --writeall --target ./pimo /usr/share/ontology/pimo/pimo.trig
nepomuk2-rcgen --writeall --target ./nao /usr/share/ontology/nepomuk/nao.trig
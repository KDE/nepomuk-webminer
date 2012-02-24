 
#include "nepomukpipe.h"

#include "../metadataparameters.h"

NepomukPipe::NepomukPipe(QObject *parent)
    : QObject(parent)
{
	
}

NepomukPipe::~NepomukPipe()
{

}

void NepomukPipe::pipeImport(QList<MetaDataParameters*> & bibEntries)
{
    emit progress(0);
    qreal perEntryProgress = (100.0/(qreal)bibEntries.size());
    qreal currentprogress = 0.0;

    foreach(MetaDataParameters *mdp, bibEntries ) {
        pipeImport(mdp);

        currentprogress += perEntryProgress;
        emit progress(currentprogress);
    }

    emit progress(100);
}

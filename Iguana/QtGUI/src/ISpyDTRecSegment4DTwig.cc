#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyDTRecSegment4DTwig.h"
#include "Iguana/QtGUI/interface/IgDrawFactoryService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <QSettings>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpyDTRecSegment4DTwig::ISpyDTRecSegment4DTwig (IgState *state, IgTwig *parent,
						const std::string &name /* = "" */)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyDTRecSegment4DTwig::onNewEvent (ISpyEventMessage& message)
{
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("DTRecSegment4D_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;

    if (IgDrawFactoryService *drawService = IgDrawFactoryService::get (state ()))
    {	
	SoSeparator *rep = dynamic_cast<SoSeparator *>(drawService->draw ("Segments", state (), "DTRecSegment4D_V1", "3D"));

	SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());
	sep->setName (SbName ("ISpyDTRecSegment4DTwig"));

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0xC0000000, rgbcomponents); // Free Speech Red
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);

	SoDrawStyle *sty = new SoDrawStyle;
	sty->style = SoDrawStyle::LINES;
	sty->lineWidth.setValue (3);
	sep->addChild (sty);
	
	sep->addChild (rep);
    }
}
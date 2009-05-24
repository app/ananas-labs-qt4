#ifndef AEXTSERVICE_H
#define AEXTSERVICE_H

#include "aextension.h"
#include "aobject.h"

/**
 * \ru
 *	\brief	Сервисные функции для доступа из Скрипта. 
 *
 * \_ru
 */
class ANANAS_EXPORT aExtSERVICE: public AExtension
{
	Q_OBJECT               
public:
	aExtSERVICE();
	~aExtSERVICE();


public slots:
	QString GetOffice() const;
	QString Generate() const;

	QString GetRcFileName() const;
	QString GetRcValue( QString paramName ) const;
};

#endif

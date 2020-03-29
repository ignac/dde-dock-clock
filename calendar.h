#ifndef CALENDAR_H
#define CALENDAR_H

#include "dde-dock/pluginsiteminterface.h"
#include <QCalendarWidget>
#include <QColor>
#include <QDate>
#include <QPen>
#include <QBrush>

class Calendar : public QCalendarWidget
{
    Q_OBJECT

public:
    Calendar(QWidget *parent = nullptr);
    ~Calendar();

    void SetHighLightColor(const QColor& color);

    QString textStyle;

    void updateButtonStyle();

protected:
    virtual void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

private:
    void dumpStructure(const QObject *obj, int spaceCount);

    QSettings m_settings;
    QColor m_backgroundColor;
    QColor m_highLightColor;

};

#endif // CALENDAR_H

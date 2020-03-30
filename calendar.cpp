#include "calendar.h"
#include "datetimeplugin.h"
#include <QPainter>
#include <QApplication>
#include <QTableView>
#include <QHeaderView>
#include <QToolButton>

Calendar::Calendar(QWidget *parent) : QCalendarWidget(parent),
      m_settings("deepin", "dde-dock-clock")
{
    m_backgroundColor = QColor(80, 80, 80, 60);

    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);

	QTableView *table = findChild<QTableView *>("qt_calendar_calendarview");
    if (table)
    {
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        table->horizontalHeader()->setDefaultSectionSize(40);
        table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        table->verticalHeader()->setDefaultSectionSize(40);
        table->setStyleSheet("QTableView {selection-background-color: transparent;}"
                             "QTableView {alternate-background-color: transparent;}");
    }

    QToolButton *year = findChild<QToolButton *>("qt_calendar_yearbutton");
    if (year)
    {
        year->setEnabled(false);
        year->setStyleSheet("QToolButton {color: white}");
    }

    QToolButton *month = findChild<QToolButton *>("qt_calendar_monthbutton");
    if (month)
    {
        month->setEnabled(false);
        month->setStyleSheet("QToolButton {color: white}");
    }

    setStyleSheet("#qt_calendar_navigationbar { background-color: rgba(20,20,20, 80); }"
                  "QCalendarWidget { background-color: rgba(20,20,20, 80); }"
                  "QCalendarWidget QAbstractItemView:enabled { background-color: rgba(20,20,20, 80); }"
                  "QToolButton::menu-indicator {image: none;}"
                  "QCalendarWidget QToolButton { font-size: 16px;}");

    dumpStructure(this, 0);
}

void Calendar::dumpStructure(const QObject *obj, int spaceCount)
{
    qDebug() << QString("%1%2 : %3")
                .arg("", spaceCount)
                .arg(obj->metaObject()->className())
                .arg(obj->objectName());

    QObjectList list = obj->children();

    foreach (QObject * child, list) {
        dumpStructure(child, spaceCount + 4);
    }
}

void Calendar::SetHighLightColor(const QColor &color)
{
    m_highLightColor = color;
}

Calendar::~Calendar()
{
}

void Calendar::updateButtonStyle()
{   
    QToolButton *prevmonth = findChild<QToolButton *>("qt_calendar_prevmonth");
    if (prevmonth)
    {
        prevmonth->setFixedSize(43, 43);
        prevmonth->setIcon(QIcon());
        prevmonth->setText("🡠");
        prevmonth->setStyleSheet(textStyle);
        prevmonth->setCursor(Qt::PointingHandCursor);
    }

    QToolButton *nextmonth = findChild<QToolButton *>("qt_calendar_nextmonth");
    if (nextmonth)
    {
        nextmonth->setFixedSize(43, 43);
        nextmonth->setIcon(QIcon());
        nextmonth->setText("🡢");
        nextmonth->setStyleSheet(textStyle);
        nextmonth->setCursor(Qt::PointingHandCursor);
    }

    if (m_settings.value("FirstDayMonday", true).toBool())
    {
        this->setFirstDayOfWeek(Qt::Monday);
    }
    else
    {
        this->setFirstDayOfWeek(Qt::Sunday);
    }
}

void Calendar::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    //QCalendarWidget::paintCell(painter, rect, date);
    QString dateDay = QString::number(date.day());

    int alfa = m_settings.value("SetAlfa", 110).toInt();

    QColor selCellColor = m_highLightColor;
    selCellColor.setAlpha(alfa);
    QColor color = QColor(255, 255, 255);
    color.setAlpha(alfa - 40);

    QFont font = qApp->font();
    QRectF drawRect(rect.adjusted( 2, 2, -2, -2));

    if (date == selectedDate() || date == QDate::currentDate())
    {
        font.setWeight(QFont::Bold);
    }

    if (date.month() == monthShown())
    {
        if (date.dayOfWeek() == Qt::Sunday || date.dayOfWeek() == Qt::Saturday)
        {
            painter->setPen(m_highLightColor);
            font.setWeight(QFont::Medium);
        }
        else
        {
            painter->setPen(Qt::white);
        }

        font.setPointSize(16);
    }
    else
    {
        painter->setPen(Qt::lightGray);
        font.setPointSize(14);
    }

    painter->setFont(font);
    painter->setRenderHints(QPainter::Antialiasing);

    if (date == selectedDate() && date == QDate::currentDate()) 
    {
        painter->setBrush(color);
        painter->drawRoundedRect(drawRect.adjusted(5.5, 5.5, -5.5, -5.5), 3, 3);
    }

    if (date == selectedDate())
    {
        painter->setBrush(selCellColor);
        painter->drawRoundedRect(drawRect, 4, 4);

        if (date != QDate::currentDate())
            painter->setPen(Qt::white);
    }
    
    if (date == QDate::currentDate())
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(color);
        painter->drawRoundedRect(drawRect, 4, 4);
        painter->setPen(Qt::white);
    }

    painter->drawText(drawRect, Qt::AlignCenter, dateDay);
}

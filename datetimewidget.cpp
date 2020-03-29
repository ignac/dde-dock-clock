/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dde-dock/constants.h"
#include "datetimewidget.h"
#include <QApplication>
#include <QPainter>
#include <QDebug>
#include <QSvgRenderer>
#include <QMouseEvent>

#define PLUGIN_STATE_KEY    "enable"

DatetimeWidget::DatetimeWidget(QWidget *parent) : QWidget(parent),
      m_settings("deepin", "dde-dock-clock"),
      m_24HourFormat(m_settings.value("24HourFormat", true).toBool())
{
    m_TimeFont = qApp->font();
    m_TimeFont.setPointSize(20);

    m_DateFont = qApp->font();
    m_DateFont.setPointSize(14);
}

bool DatetimeWidget::enabled()
{
    return m_settings.value(PLUGIN_STATE_KEY, true).toBool();
}

void DatetimeWidget::setEnabled(const bool b)
{
    m_settings.setValue(PLUGIN_STATE_KEY, b);
}

void DatetimeWidget::toggleClock()
{
    update();
}

void DatetimeWidget::GetDateTimeFormat(QString &dateFormat, QString &timeFormat) const
{
    QString format = m_settings.value("Format","HH:mm\nyyyy/M/d ddd").toString().replace("\\n","\n");

    QStringList formats = format.split("\n");
    timeFormat = formats[0];
    dateFormat = formats[1];
}

void DatetimeWidget::toggleHourFormat()
{
    m_24HourFormat = !m_24HourFormat;
    m_settings.setValue("24HourFormat", m_24HourFormat);
    update();
    emit requestUpdateGeometry();
}

QSize DatetimeWidget::sizeHint() const
{
    QFontMetrics timeMetrics(m_TimeFont);
    QFontMetrics dateMetrics(m_DateFont);

    QString timeFormat, dateFormat;
    this->GetDateTimeFormat(dateFormat, timeFormat);

    QString format = m_settings.value("Format", "HH:mm\nM/d ddd").toString();
    if (format.contains("\\n"))
    {
        QRect timeRect = timeMetrics.boundingRect(timeFormat);
        QRect dateRect = dateMetrics.boundingRect(dateFormat);

        return QSize(qMax(timeRect.width(), dateRect.width()) + 10, timeRect.height() * 2);
    }
    else
    {
        return QSize(timeMetrics.boundingRect(format).width(), timeMetrics.boundingRect("8").height() * 2);
    }
}

void DatetimeWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

void DatetimeWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    const QDateTime current = QDateTime::currentDateTime();
    QFontMetrics FM(m_TimeFont);

    QString timeFormat, dateFormat;
    this->GetDateTimeFormat(dateFormat, timeFormat);

    QRect timeRect = rect();
    timeRect.setHeight(FM.boundingRect("8").height());

    QRect dateRect = rect();
    dateRect.setHeight(dateRect.height() - timeRect.height());
    dateRect.setY(dateRect.y() + 2*timeRect.height());

    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setFont(m_TimeFont);
    painter.drawText(timeRect, Qt::AlignCenter, current.toString(timeFormat));

    painter.setFont(m_DateFont);
    painter.drawText(dateRect, Qt::AlignCenter, current.toString(dateFormat));
}

const QPixmap DatetimeWidget::loadSvg(const QString &fileName, const QSize size)
{
    const auto ratio = qApp->devicePixelRatio();

    QPixmap pixmap(size * ratio);
    QSvgRenderer renderer(fileName);
    pixmap.fill(Qt::transparent);

    QPainter painter;
    painter.begin(&pixmap);
    renderer.render(&painter);
    painter.end();

    pixmap.setDevicePixelRatio(ratio);

    return pixmap;
}

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

#ifndef DATETIMEWIDGET_H
#define DATETIMEWIDGET_H

#include <QWidget>
#include <QSettings>

class DatetimeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DatetimeWidget(QWidget *parent = 0);
    bool is24HourFormat() const { return m_24HourFormat; }
    bool enabled();
    void setEnabled(const bool b);

signals:
    void requestUpdateGeometry() const;
    void requestContextMenu() const;

public slots:
    void Refresh();
    void toggleClock();

private:
    QSize sizeHint() const;
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
    const QPixmap loadSvg(const QString &fileName, const QSize size);

    void GetDateTimeFormats(QString &dateFormat, QString &timeFormat) const;
    void GetDateTimeFonts(QFont &dateFont, QFont &timeFont) const;

private:
    QSettings m_settings;
    bool m_24HourFormat;
};

#endif // DATETIMEWIDGET_H

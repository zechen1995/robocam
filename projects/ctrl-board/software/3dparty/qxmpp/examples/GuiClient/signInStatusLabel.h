/*
 * Copyright (C) 2008-2012 The QXmpp developers
 *
 * Author:
 *	Manjeet Dahiya
 *
 * Source:
 *	http://code.google.com/p/qxmpp
 *
 * This file is a part of QXmpp library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */


#ifndef SIGNINSTATUSLABEL_H
#define SIGNINSTATUSLABEL_H

#include <QLabel>
#include <QTimer>

class signInStatusLabel : public QLabel
{
    Q_OBJECT

public:
    enum Option
    {
        None = 0,
        WithProgressEllipsis,
        CountDown
    };
    signInStatusLabel(QWidget* parent = 0);

    void setCustomText(const QString& text, signInStatusLabel::Option op = None,
                       int countDown = 0);

//    QSize sizeHint() const;

private slots:
    void timeout();

private:
    QTimer m_timer;
    signInStatusLabel::Option m_option;
    QString m_text;
    QString m_postfix;
    int m_countDown;
};

#endif // SIGNINSTATUSLABEL_H

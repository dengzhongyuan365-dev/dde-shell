// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "applet.h"
#include "private/applet_p.h"
#include "private/appletproxy_p.h"

#include <QLoggingCategory>
#include <QUuid>

DS_BEGIN_NAMESPACE

DCORE_USE_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(dsLog)

DAppletPrivate::DAppletPrivate(DApplet *qq)
    : DTK_CORE_NAMESPACE::DObjectPrivate(qq)
{
}

DAppletPrivate::~DAppletPrivate()
{
    if (m_rootObject) {
        m_rootObject->deleteLater();
    }
}

DAppletProxy *DAppletPrivate::appletProxy() const
{
    if (!m_proxy) {
        auto meta = const_cast<DAppletPrivate *>(this)->q_func()->createProxyMeta();
        // TODO DAppletMetaProxy can't router all operator of QMetaObject::invokeMethod,
        // so DAppletProxy is createProxyMeta's object temporally.
        // const_cast<DAppletPrivate *>(this)->m_proxy =
        //         new DAppletMetaProxy(meta, const_cast<DAppletPrivate *>(this)->q_func());
        const_cast<DAppletPrivate *>(this)->m_proxy =
                reinterpret_cast<DAppletProxy *>(meta);
    }
    return m_proxy;
}

DApplet::DApplet(QObject *parent)
    : DApplet(*new DAppletPrivate(this), parent)
{
}

DApplet::DApplet(DAppletPrivate &dd, QObject *parent)
    : QObject(parent)
    , DObject(dd)
{

}

DApplet::~DApplet()
{
    qDebug(dsLog) << "Destroyed applet:" << pluginId() << id();
}

QString DApplet::id() const
{
    D_DC(DApplet);
    return d->m_data.id();
}

void DApplet::setMetaData(const DPluginMetaData &metaData)
{
    D_D(DApplet);
    d->m_metaData = metaData;
}

void DApplet::setAppletData(const DAppletData &data)
{
    D_D(DApplet);
    d->m_data = data;
}

void DApplet::setRootObject(QObject *root)
{
    D_D(DApplet);
    if (d->m_rootObject == root)
        return;
    d->m_rootObject = root;
    Q_EMIT rootObjectChanged();
}

QString DApplet::pluginId() const
{
    D_DC(DApplet);
    return d->m_metaData.pluginId();
}

DPluginMetaData DApplet::pluginMetaData() const
{
    D_DC(DApplet);
    return d->m_metaData;
}

DAppletData DApplet::appletData() const
{
    D_DC(DApplet);
    return d->m_data;
}

QObject *DApplet::rootObject() const
{
    D_DC(DApplet);
    return d->m_rootObject;
}

DApplet *DApplet::parentApplet() const
{
    return qobject_cast<DApplet *>(parent());
}

bool DApplet::load()
{
    return true;
}

bool DApplet::init()
{
    return true;
}

QObject *DApplet::createProxyMeta()
{
    D_DC(DApplet);
    return this;
}

DS_END_NAMESPACE

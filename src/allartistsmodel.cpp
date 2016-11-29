/*
 * Copyright 2015-2016 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "allartistsmodel.h"
#include "databaseinterface.h"
#include "musicartist.h"

#include <QtCore/QUrl>
#include <QtCore/QTimer>
#include <QtCore/QPointer>
#include <QtCore/QVector>

class AllArtistsModelPrivate
{
public:

    AllArtistsModelPrivate()
    {
    }

    QString mFilter;

    DatabaseInterface *mMusicDatabase = nullptr;

    QVector<MusicArtist> mAllArtists;

    int mArtistsCount = 0;

    bool mUseLocalIcons = false;

};

AllArtistsModel::AllArtistsModel(QObject *parent) : QAbstractItemModel(parent), d(new AllArtistsModelPrivate)
{
}

AllArtistsModel::~AllArtistsModel()
{
    delete d;
}

int AllArtistsModel::rowCount(const QModelIndex &parent) const
{
    auto albumCount = 0;

    if (!d->mMusicDatabase) {
        return albumCount;
    }

    if (parent.isValid()) {
        return albumCount;
    }

    albumCount = d->mArtistsCount;

    return albumCount;
}

QHash<int, QByteArray> AllArtistsModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[static_cast<int>(ColumnsRoles::NameRole)] = "name";
    roles[static_cast<int>(ColumnsRoles::AlbumsCountRole)] = "albumsCount";
    roles[static_cast<int>(ColumnsRoles::ImageRole)] = "image";
    roles[static_cast<int>(ColumnsRoles::IdRole)] = "id";

    return roles;
}

Qt::ItemFlags AllArtistsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant AllArtistsModel::data(const QModelIndex &index, int role) const
{
    auto result = QVariant();

    if (!d->mMusicDatabase) {
        return result;
    }

    const auto artistsCount = d->mArtistsCount;

    if (!index.isValid()) {
        return result;
    }

    if (index.column() != 0) {
        return result;
    }

    if (index.row() < 0) {
        return result;
    }

    if (index.parent().isValid()) {
        return result;
    }

    if (index.internalId() != 0) {
        return result;
    }

    if (index.row() < 0 || index.row() >= artistsCount) {
        return result;
    }

    ColumnsRoles convertedRole = static_cast<ColumnsRoles>(role);

    switch(convertedRole)
    {
    case ColumnsRoles::NameRole:
        result = d->mAllArtists[index.row()].name();
        break;
    case ColumnsRoles::AlbumsCountRole:
        break;
    case ColumnsRoles::ImageRole:
        break;
    case ColumnsRoles::IdRole:
        break;
    }

    return result;
}

QModelIndex AllArtistsModel::index(int row, int column, const QModelIndex &parent) const
{
    auto result = QModelIndex();

    if (column != 0) {
        return result;
    }

    if (parent.isValid()) {
        return result;
    }

    result = createIndex(row, column);

    return result;
}

QModelIndex AllArtistsModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)

    auto result = QModelIndex();

    return result;
}

int AllArtistsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}

DatabaseInterface *AllArtistsModel::databaseInterface() const
{
    return d->mMusicDatabase;
}

QString AllArtistsModel::filter() const
{
    return d->mFilter;
}

void AllArtistsModel::setDatabaseInterface(DatabaseInterface *musicDatabase)
{
    if (d->mMusicDatabase == musicDatabase) {
        return;
    }

    if (d->mMusicDatabase) {
        disconnect(d->mMusicDatabase);
    }

    d->mMusicDatabase = musicDatabase;

    if (d->mMusicDatabase) {
        connect(d->mMusicDatabase, &DatabaseInterface::beginAlbumAdded, this, &AllArtistsModel::beginArtistAdded);
        connect(d->mMusicDatabase, &DatabaseInterface::endAlbumAdded, this, &AllArtistsModel::endArtistAdded);
    }

    beginResetModel();
    d->mAllArtists = d->mMusicDatabase->allArtists(d->mFilter);
    d->mArtistsCount = d->mAllArtists.count();
    endResetModel();

    emit databaseInterfaceChanged();
}

void AllArtistsModel::setFilter(const QString &filter)
{
    if (d->mFilter == filter) {
        return;
    }

    beginResetModel();

    d->mFilter = filter;
    d->mAllArtists = d->mMusicDatabase->allArtists(d->mFilter);
    d->mArtistsCount = d->mAllArtists.count();

    endResetModel();

    Q_EMIT filterChanged();
}

void AllArtistsModel::beginArtistAdded(QVector<qulonglong> newArtists)
{
    Q_UNUSED(newArtists);
}

void AllArtistsModel::endArtistAdded(QVector<qulonglong> newArtists)
{
    beginResetModel();
    d->mAllArtists = d->mMusicDatabase->allArtists(d->mFilter);
    d->mArtistsCount = d->mAllArtists.count();
    endResetModel();
}

#include "moc_allartistsmodel.cpp"
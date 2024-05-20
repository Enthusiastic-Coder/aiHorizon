#include <QDir>
#include <QString>
#include <QStringList>
#include <QDebug>

#define androidAssetFolder "assets:/"

static void makePathForDestination(const QString& filePath)
{
    QFileInfo fi(filePath);
    QDir dir(QDir::currentPath());
    dir.mkpath(fi.absolutePath());
}

static void ExtractAsset(const QString filePath, const QString& folder=androidAssetFolder, bool bOverwrite=true)
{    
    QString fullFilePath= QString("%1%2").arg(folder).arg(filePath);

    if(bOverwrite)
        QFile::remove(filePath);

    makePathForDestination(filePath);

    QFile::copy(fullFilePath, filePath);
}


class AssetTempExtractor
{
public:
    AssetTempExtractor( const QStringList& fileList, const QString& folder=androidAssetFolder)
        :_fileList(fileList), _folder(folder)
    {
#ifdef Q_OS_WIN
    if( folder == androidAssetFolder )
        return;
#endif
        for(const QString& fileName:_fileList)
            ExtractAsset(fileName, androidAssetFolder);
    }

    ~AssetTempExtractor()
    {
#ifdef Q_OS_WIN
    if( _folder == androidAssetFolder )
        return;
#endif
        for(const QString& fileName:_fileList)
            QFile::remove(fileName);
    }

private:
    const QStringList& _fileList;
    QString _folder;
};

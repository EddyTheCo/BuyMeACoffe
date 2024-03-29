#include "model.hpp"

Message_model::Message_model(QObject *parent)
    : QAbstractListModel(parent),monitor_(new Monitor(this))
{
    QObject::connect(monitor_,&Monitor::new_output,this,&Message_model::add_message);
    QObject::connect(monitor_,&Monitor::outputChanged,this,&Message_model::rm_message);
    QObject::connect(monitor_,&Monitor::restarted,this,&Message_model::reset);

}
void Message_model::add_message(QString outid, QJsonObject var)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_messages.insert(0,new Message_box(outid,var));
    emit countChanged(count());
    endInsertRows();
}
void Message_model::rm_message(QString outid)
{
    for(auto i=0;i<m_messages.size();i++)
    {
        if(m_messages[i]->outid_==outid)
        {
            beginRemoveRows(QModelIndex(), i, i);
            m_messages.remove(i);
            emit countChanged(count());
            endRemoveRows();
            return;
        }
    }
}
void Message_model::reset(void)
{
    beginRemoveRows(QModelIndex(),0,m_messages.size()-1);
    m_messages.clear();
    emit countChanged(count());
    endRemoveRows();
}
int Message_model::count() const
{
    return m_messages.size();
}
int Message_model::rowCount(const QModelIndex &p) const
{
    Q_UNUSED(p)
    return m_messages.size();
}
QHash<int, QByteArray> Message_model::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[jsobRole] = "jsob";
    return roles;
}
QVariant Message_model::data(const QModelIndex &index, int role) const
{
    return m_messages[index.row()]->property(roleNames().value(role));
}
QModelIndex Message_model::index(int row, int column , const QModelIndex &parent ) const
{
    return createIndex(row,column);
}

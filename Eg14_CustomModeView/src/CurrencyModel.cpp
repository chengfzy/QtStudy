#include "CurrencyModel.h"

CurrencyModel::CurrencyModel(QObject* parent) : QAbstractTableModel(parent) {}

void CurrencyModel::setCurrencyMap(const QMap<QString, double>& map) {
    beginResetModel();
    currencyMap_ = map;
    endResetModel();
}

int CurrencyModel::rowCount(const QModelIndex& parent) const { return currencyMap_.count(); }

int CurrencyModel::columnCount(const QModelIndex& parent) const { return currencyMap_.count(); }

QVariant CurrencyModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        QString rowCurrency = currencyAt(index.row());
        QString columnCurrency = currencyAt(index.column());
        if (currencyMap_.value(rowCurrency) == 0.0) {
            return "####";
        }
        double amount = currencyMap_.value(columnCurrency) / currencyMap_.value(rowCurrency);
        return QString("%1").arg(amount, 0, 'f', 4);
    }
    return QVariant();
}

QVariant CurrencyModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    return currencyAt(section);
}

Qt::ItemFlags CurrencyModel::flags(const QModelIndex& index) const {
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (index.row() != index.column()) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

bool CurrencyModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (index.isValid() && index.row() != index.column() && role == Qt::EditRole) {
        QString columnCurrency = headerData(index.column(), Qt::Horizontal, Qt::DisplayRole).toString();
        QString rowCurrency = headerData(index.row(), Qt::Vertical, Qt::DisplayRole).toString();
        currencyMap_.insert(columnCurrency, value.toDouble() * currencyMap_.value(rowCurrency));
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

QString CurrencyModel::currencyAt(int offset) const {
    auto it = currencyMap_.begin();
    if (offset > 0)
        while (offset--) {
            ++it;
        }
    else {
        while (offset++) {
            --it;
        }
    }
    return it.key();
}
#pragma once
#include <QAbstractTableModel>

class CurrencyModel : public QAbstractTableModel {
  public:
    explicit CurrencyModel(QObject* parent = nullptr);

  public:
    void setCurrencyMap(const QMap<QString, double>& map);

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;

    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

  private:
    QString currencyAt(int offset) const;

  private:
    QMap<QString, double> currencyMap_;
};

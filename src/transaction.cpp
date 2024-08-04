#include "transaction.h"
#include <QDateTime>

Transaction::Transaction()
    : transaction_type(""),
    description(""), transaction_id(0), sender_id(0), receiver_id(0), amount(0.0) {}

Transaction::Transaction(const QString& description_, int sender_id_, int receiver_id_, float amount_)
    : description(description_), transaction_id(0), sender_id(sender_id_), receiver_id(receiver_id_), amount(amount_) {}

Transaction::Transaction(int transaction_id_, const QString& transaction_type_, const QString& description_, int sender_id_, int receiver_id_, float amount_, const QString& created_at_)
    : transaction_type(transaction_type_), description(description_), transaction_id(transaction_id_), sender_id(sender_id_), receiver_id(receiver_id_), amount(amount_), created_at(created_at_) {}

void Transaction::setTransactionType(QString transaction_type_) {
     transaction_type = transaction_type_;
}

QString Transaction::getDescription() const {
    return description;
}

QString Transaction::getTransactionType() const {
    return transaction_type;
}

QString Transaction::getCreatedAt() const {
    return created_at;
}

float Transaction::getAmount() const {
    return amount;
}

int Transaction::getSenderId() const {
    return sender_id;
}

int Transaction::getReceiverId() const {
    return receiver_id;
}

int Transaction::getTransactionId() const {
    return transaction_id;
}

QString Transaction::getLogEntry() const {
    QString logEntry;
    //change to account numbers and username later
    if (transaction_type == "send") {
        logEntry = QString("Account ID: %1 sent $%2 to Receiver ID: %3")
                       .arg(sender_id)
                       .arg(amount)
                       .arg(receiver_id);
    } else if (transaction_type == "transfer") {
        logEntry = QString("Account ID: %1 transferred $%2 to Receiver ID: %3")
                       .arg(sender_id)
                       .arg(amount)
                       .arg(receiver_id);
    } else {
        return "Transaction incomplete";
    }

    return logEntry;
}

QDebug operator << (QDebug dbg, const Transaction& transaction) {
    QDebugStateSaver saver(dbg);
    dbg.nospace();


    dbg << "Transaction ID: " << transaction.transaction_id << "\n"
        << "Transaction Type: " << transaction.transaction_type << "\n"
        << "Description: " << transaction.description << "\n"
        << "Sender Account ID: " << transaction.sender_id << "\n"
        << "Receiver Account ID: " << transaction.receiver_id << "\n"
        << "Amount: " << transaction.amount << "\n"
        << "Created At: " << transaction.created_at;

    return dbg;
}


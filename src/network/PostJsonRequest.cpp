#include "PostJsonRequest.h"
#include <QNetworkRequest>
#include <QJsonDocument>

namespace compilerExplorer {
namespace network{

PostJsonRequest::PostJsonRequest()
	: GetRequest() {

}

std::unique_ptr<QNetworkReply> PostJsonRequest::sendRequest(QNetworkAccessManager *manager) {
	if(address().isEmpty() || !manager)
		return nullptr;
	QNetworkRequest request;
	QUrl url(address());
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setUrl(url);
	return std::unique_ptr<QNetworkReply>(manager->post(request, QJsonDocument(jsonRequest()).toJson()));
}

QJsonObject PostJsonRequest::jsonRequest() const {
	return jsonRequest(parameters());
}

QJsonObject PostJsonRequest::jsonRequest(const std::map<QString, QString> &parameters) const {
	QJsonObject result;
	for(const auto &param : parameters) {
		result.insert(param.first, param.second);
	}
	QJsonObject options;
	options["userArguments"] = mUserArgs;
	if(!filters().isEmpty()) {
		QJsonObject enabledFilters;
		for(auto filter : filters()) {
			enabledFilters.insert(filter, "true");
		}
		options["filters"] = enabledFilters;
	}
	result["options"] = options;
	return result;
}

QStringList PostJsonRequest::filters() const {
	return mFilters;
}

void PostJsonRequest::setFilters(const QStringList &filters) {
	mFilters = filters;
}

void PostJsonRequest::setFiltersKey(const QString &filtersKey) {
	mFiltersKey = filtersKey;
}

void PostJsonRequest::setUserArguments(const QString &opt) {
	mUserArgs = opt;
}

QString PostJsonRequest::requestName() const {
	return QObject::tr("PostJsonRequest");
}
}
}

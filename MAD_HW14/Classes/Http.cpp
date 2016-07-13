#include "Http.h"

ChainHttpClient::ChainHttpClient() : httpReq(nullptr) {}

ChainHttpClient::~ChainHttpClient() {}

ChainHttpClient * ChainHttpClient::setUrl(const char * url) {
	httpReq->setUrl(url);
	return this;
}

ChainHttpClient * ChainHttpClient::addQuery(const char * key, const char * value) {
	queries.insert(std::pair<std::string, std::string>(std::string(key), std::string(value)));
	return this;
}

ChainHttpClient * ChainHttpClient::setCharBufferContent(const char * buffer, size_t len) {
	httpReq->setRequestData(buffer, len != UNSET_CONTENT_LENGTH ? len : strlen(buffer));
	return this;
}

ChainHttpClient * ChainHttpClient::addHeader(const char * key, const char * value) {
	headers.insert(std::pair<std::string, std::string>(key, value));
	return this;
}

ChainHttpClient * ChainHttpClient::GET(const ccHttpRequestCallback & callback, bool autoRelease) {
	httpReq->setResponseCallback(callback);
	httpReq->setRequestType(HttpRequest::Type::GET);
	addQueries();
	addHeaders();
	HttpClient::getInstance()->send(httpReq);
	httpReq->release();
	return this;
}

ChainHttpClient * ChainHttpClient::POST(const ccHttpRequestCallback & callback, bool autoRelease) {
	httpReq->setResponseCallback(callback);
	httpReq->setRequestType(HttpRequest::Type::POST);
	addQueries();
	addHeaders();
	HttpClient::getInstance()->send(httpReq);
	httpReq->release();
	return this;
}

bool ChainHttpClient::init() {
	httpReq = new HttpRequest();
	return true;
}

void ChainHttpClient::addQueries() {
	if (queries.size() > 0) {
		std::string q(httpReq->getUrl());
		q.append("?");
		for (std::map<std::string, std::string>::iterator i = queries.begin(); i != queries.end(); i++) {
			q.append(i->first.c_str());
			q.append("=");
			q.append(i->second.c_str());
		}
		httpReq->setUrl(q.c_str());
	}
}

void ChainHttpClient::addHeaders() {
	std::vector<std::string> h;
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it) {
		h.push_back(it->first + ": " + it->second);
	}
	httpReq->setHeaders(h);
}

ChainHttpResponse::ChainHttpResponse(HttpResponse * res) : response(res) {
	if (response) {
		response->retain();

		// Headers
		std::string rawHeaders(response->getResponseHeader()->data(), response->getResponseHeader()->size());
		//rawHeaders.push_back('\0');
		
		unsigned start = 0, end, col;
		while (start < rawHeaders.size()) {
			end = rawHeaders.find("\r\n", start);
			// Two continuing \r\n found
			if (end == start) break;
			std::string current(rawHeaders.substr(start, end - start));
			start = end + 2;
			// Find :
			col = current.find(": ");
			if (col == std::string::npos) continue;
			headers.insert(std::pair<std::string, std::string>(current.substr(0, col), current.substr(col + 2)));
		}

		// DataString
		dataString = std::string(response->getResponseData()->data(), response->getResponseData()->size());
		//dataString.push_back('\0');
	}
}

ChainHttpResponse::~ChainHttpResponse() {
	if (response) {
		response->release();
	}
}

HttpResponse * ChainHttpResponse::getResponse() {
	return response;
}

const char * ChainHttpResponse::getHeader(const char * key) {
	return headers.find(key)->second.c_str();
}

const char * ChainHttpResponse::getDataString() {
	return dataString.c_str();
}

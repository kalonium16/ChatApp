#pragma once
#include <iostream>
namespace Network {
	struct RequestData
	{
		uint32_t reciverId{};
		uint32_t senderId{};
	};

	template <typename T>
	struct RequestHeader
	{
		T id{};
		uint32_t size{}; 
		RequestData reqData{};
	};

	template <typename T>
	class Request {
	public:
		size_t size() {
			return m_body.size();
		}

		RequestHeader<T>& getHeader() {
			return m_header;
		}

		const RequestHeader<T>& getHeader() const {
			return m_header;
		}

		std::string& getBody() {
			return m_body;
		}

		const std::string& getBody() const {
			return m_body;
		}

		void clear() {
			m_body.clear();
			m_header = RequestHeader<T>{};
		}

		friend std::ostream& operator<< (std::ostream& os, const Request<T>& req) {
			os << "ID: " << static_cast<int>(req.getHeader().id) << " Size: " << req.getHeader().size;
			return os;
		}

		template <typename DataType>
		friend Request<T>& operator<<(Request<T>& req, const DataType& data) {
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be added to Request");
			size_t size = req.m_body.size();
			req.m_body.resize(req.m_body.size() + sizeof(DataType));

			std::memcpy(req.m_body.data() + size, &data, sizeof(DataType));

			req.m_header.size = req.size();

			return req;
		}
		
		friend Request<T>& operator<<(Request<T>& req, const std::string& str) {
			size_t size = str.size();
			req.getHeader().size = size;
			req.getBody().resize(size);
			std::memcpy(req.getBody().data(), str.data(), size);
			return req;
		};

		
		template <typename DataType>
		friend Request<T>& operator>>(Request<T>& req,DataType& data) {
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be added to Request");
			size_t size = req.m_body.size() - sizeof(DataType);

			std::memcpy(&data, req.m_body.data() + size, sizeof(DataType));

			req.m_body.resize(size);


			req.m_header.size = req.size();

			return req;
		}
		
		friend Request<T>& operator>>(Request<T>& req, std::string& str) {
			
			str.resize(req.getHeader().size);
			std::memcpy(str.data(), req.m_body.data(), str.size());
			return req;
		}

	private:
		RequestHeader<T> m_header{};
		std::string m_body;


	};

	template <typename T>
	class Connection;

	template <typename T>
	struct OwnedRequest {
		std::shared_ptr<Connection<T>> clientReq = nullptr;
		Request<T> req;

		friend std::ostream& operator<< (std::ostream& os, const OwnedRequest<T>& req) {
			os << req;
			return os;
		};

	};

}
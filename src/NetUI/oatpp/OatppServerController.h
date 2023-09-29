#pragma once
#include "OatppDTOs.h"
#include <unistd.h>
#include <iostream>
#include <exceptions/database_exceptions.h>

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/core/utils/String.hpp"
#include "IBLFacade.h"
#include OATPP_CODEGEN_BEGIN(ApiController)"oatpp/codegen/ApiController_define.hpp"


class OatppServerController : public oatpp::web::server::api::ApiController {

protected:
    OatppServerController(const std::shared_ptr<ObjectMapper>& objectMapper)
            : oatpp::web::server::api::ApiController(objectMapper)
    {}
public:
    static IBLFacadePtr facade;
    static std::shared_ptr<OatppServerController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                      objectMapper)){
        return std::shared_ptr<OatppServerController>(new OatppServerController(objectMapper));
    }

    ENDPOINT_ASYNC("GET", "/api/v1/persons/{id}", PersonGetPoint ) {

    ENDPOINT_ASYNC_INIT(PersonGetPoint)

        Action act() override {
            int id;
            try {
                id = std::stoi(request->getPathVariable("id")->c_str());
            } catch (const std::invalid_argument& e) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid ID: must be integer!");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            try {
            auto u = facade->GetUserByID(id);
                auto PersonDto = PersonResponseDto::createShared();
                PersonDto->name = u.name;
                PersonDto->work = u.work;
                PersonDto->address = u.address;
                PersonDto->age = u.age;
                PersonDto->id = id;
                return _return(controller->createDtoResponse(Status::CODE_200, PersonDto));
            } catch (DatabaseException) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Not found with such ID!";
                return _return(controller->createDtoResponse(Status::CODE_404, dto));
            }
        }

    };
    ENDPOINT_ASYNC("GET", "/api/v1/persons", PersonsGetPoint ) {

    ENDPOINT_ASYNC_INIT(PersonsGetPoint)
        Action act() override {
            try {
                auto users = facade->GetUsers();
                oatpp::Vector<oatpp::Object<PersonResponseDto>> dtoVector({});
                for (const auto & u : users) {
                    auto PersonDto = PersonResponseDto::createShared();
                    PersonDto->name = u.name;
                    PersonDto->work = u.work;
                    PersonDto->address = u.address;
                    PersonDto->age = u.age;
                    PersonDto->id = u.id;
                    dtoVector->push_back(PersonDto);
                }
                return _return(controller->createDtoResponse(Status::CODE_200,  dtoVector));
            } catch (DatabaseException) {
                return _return(controller->createResponse(Status::CODE_500));
            }
        }

    };
    ENDPOINT_ASYNC("POST", "/api/v1/persons", PersonPostPoint ) {

    ENDPOINT_ASYNC_INIT(PersonPostPoint)
        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&PersonPostPoint::returnPersonResponse);
        }
        Action returnPersonResponse( const oatpp::String& str) {
            oatpp::Object<PersonRequestDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<PersonRequestDto>>(str);
            } catch (const oatpp::parser::ParsingError& error){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request body: " + error.getMessage());
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            User u(-1, body->name, body->address, body->work, body->age);
            int id;
            try {
                id = facade->CreateUser(u);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Internal server error: " + err.GetInfo();
                return _return(controller->createDtoResponse(Status::CODE_500, dto));
            }
            auto PersonDto = PersonResponseDto::createShared();
            PersonDto->name = u.name;
            PersonDto->work = u.work;
            PersonDto->address = u.address;
            PersonDto->age = u.age;
            PersonDto->id = id;
            auto response = controller->createResponse(Status::CODE_201);
            response->putHeader("Location", "/api/v1/persons/" + std::to_string(id));
            return _return(response);
        }

    };
    ENDPOINT_ASYNC("PATCH", "/api/v1/persons/{id}", PersonUpdatePoint ) {

    ENDPOINT_ASYNC_INIT(PersonUpdatePoint)

        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&PersonUpdatePoint::returnPersonResponse);
        }
        Action returnPersonResponse( const oatpp::String& str) {
            oatpp::Object<PersonRequestDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<PersonRequestDto>>(str);
            } catch (const oatpp::parser::ParsingError& error){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request body: " + error.getMessage());
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            int id;
            try {
                id = std::stoi(request->getPathVariable("id")->c_str());
            } catch (const std::invalid_argument& e) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid ID: must be integer!");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            User u_old;
            try {
                u_old = facade->GetUserByID(id);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Not found with such ID!";
                return _return(controller->createDtoResponse(Status::CODE_404, dto));
            }

            User u(id,
                   body->name ? *body->name : u_old.name,
                   body->address ? *body->address : u_old.address,
                   body->work ? *body->work : u_old.work,
                   body->age != nullptr ? *body->age : u_old.age);
            try {
                facade->UpdateUserByID(id, u);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Internal server error: " + err.GetInfo();
                return _return(controller->createDtoResponse(Status::CODE_500, dto));
            }
            auto PersonDto = PersonResponseDto::createShared();
            PersonDto->name = u.name;
            PersonDto->work = u.work;
            PersonDto->address = u.address;
            PersonDto->age = u.age;
            PersonDto->id = id;
            return _return(controller->createDtoResponse(Status::CODE_200, PersonDto));
        }

    };
    ENDPOINT_ASYNC("DELETE", "/api/v1/persons/{id}", PersonDeletePoint ) {
    ENDPOINT_ASYNC_INIT(PersonDeletePoint)

        Action act() override {
            int id;
            try {
                id = std::stoi(request->getPathVariable("id")->c_str());
            } catch (const std::invalid_argument& e) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid ID: must be integer!");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }

            try {
                facade->GetUserByID(id);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Not found with such ID!";
                return _return(controller->createDtoResponse(Status::CODE_404, dto));
            }

            try {
                facade->DeleteUserByID(id);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Internal server error: " + err.GetInfo();
                return _return(controller->createDtoResponse(Status::CODE_500, dto));
            }

            return _return(controller->createResponse(Status::CODE_204));
        }

    };

};

#include OATPP_CODEGEN_BEGIN(ApiController)"oatpp/codegen/ApiController_define.hpp"

using OatppServerControllerPtr = std::shared_ptr<OatppServerController>;


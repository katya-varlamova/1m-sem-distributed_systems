#pragma once
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)"oatpp/codegen/DTO_define.hpp"

class PersonRequestDto: public oatpp::DTO {
    DTO_INIT(PersonRequestDto, DTO)

    DTO_FIELD(String, name);
    DTO_FIELD(Int64, age);
    DTO_FIELD(String, address);
    DTO_FIELD(String, work);
};
class PersonResponseDto: public oatpp::DTO {
    DTO_INIT(PersonResponseDto, DTO)

    DTO_FIELD(Int64, id);
    DTO_FIELD(String, name);
    DTO_FIELD(Int64, age);
    DTO_FIELD(String, address);
    DTO_FIELD(String, work);
};
class PersonsResponseDto: public oatpp::DTO {
    DTO_INIT(PersonsResponseDto, DTO)
    DTO_FIELD(Vector<Object<PersonResponseDto>>, persons);
};
class ValidationErrorResponse: public oatpp::DTO {
    DTO_INIT(ValidationErrorResponse, DTO)

    DTO_FIELD(String, message);
    DTO_FIELD(Vector<String>, errors);
};
class ErrorResponse: public oatpp::DTO {
    DTO_INIT(ErrorResponse, DTO)

    DTO_FIELD(String, message);
};
#include OATPP_CODEGEN_END(DTO)"oatpp/codegen/DTO_undef.hpp"



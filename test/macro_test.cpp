#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "jinja2cpp/template.h"
#include "test_tools.h"

using namespace jinja2;

TEST(MacroTest, SimpleMacro)
{
    std::string source = R"(
{% macro test %}
Hello World!
{% endmacro %}
{{ test() }}{{ test() }}
)";

    Template tpl;
    auto parseRes = tpl.Load(source);
    EXPECT_TRUE(parseRes.has_value());
    if (!parseRes)
    {
        std::cout << parseRes.error() << std::endl;
        return;
    }

    std::string result = tpl.RenderAsString(PrepareTestData());
    std::cout << result << std::endl;
    std::string expectedResult = R"(
Hello World!
Hello World!

)";
    EXPECT_EQ(expectedResult, result);
}

TEST(MacroTest, OneParamMacro)
{
    std::string source = R"(
{% macro test(param) %}
-->{{ param }}<--
{% endmacro %}
{{ test('Hello') }}{{ test(param='World!') }}
)";

    Template tpl;
    auto parseRes = tpl.Load(source);
    EXPECT_TRUE(parseRes.has_value());
    if (!parseRes)
    {
        std::cout << parseRes.error() << std::endl;
        return;
    }

    std::string result = tpl.RenderAsString(PrepareTestData());
    std::cout << result << std::endl;
    std::string expectedResult = R"(
-->Hello<--
-->World!<--

)";
    EXPECT_EQ(expectedResult, result);
}

TEST(MacroTest, OneDefaultParamMacro)
{
    std::string source = R"(
{% macro test(param='Hello') %}
-->{{ param }}<--
{% endmacro %}
{{ test() }}{{ test('World!') }}
)";

    Template tpl;
    auto parseRes = tpl.Load(source);
    EXPECT_TRUE(parseRes.has_value());
    if (!parseRes)
    {
        std::cout << parseRes.error() << std::endl;
        return;
    }

    std::string result = tpl.RenderAsString(PrepareTestData());
    std::cout << result << std::endl;
    std::string expectedResult = R"(
-->Hello<--
-->World!<--

)";
    EXPECT_EQ(expectedResult, result);
}

TEST(MacroTest, MacroVariables)
{
    std::string source = R"(
{% macro test(param1='Hello', param2, param3='World') %}
name: {{ name }}
arguments: {{ arguments | pprint }}
defaults: {{ defaults | pprint }}
varargs: {{ varargs | pprint }}
kwargs: {{ kwargs | pprint }}
{% endmacro %}
{{ test(1, 2, param3=3, 4, extraValue=5, 6) }}
)";

    Template tpl;
    auto parseRes = tpl.Load(source);
    EXPECT_TRUE(parseRes.has_value());
    if (!parseRes)
    {
        std::cout << parseRes.error() << std::endl;
        return;
    }

    std::string result = tpl.RenderAsString(PrepareTestData());
    std::cout << result << std::endl;
    std::string expectedResult = R"(
name: test
arguments: ['param1', 'param2', 'param3']
defaults: ['Hello', none, 'World']
varargs: [4, 6]
kwargs: {'extraValue': 5}

)";
    EXPECT_EQ(expectedResult, result);
}

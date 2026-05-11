#include <unity.h>
#include <cstring>
#include "PayloadParser.h"

void setUp(void) {}
void tearDown(void) {}

static bool parseStr(const char *json, OrderEvent &evt)
{
    return PayloadParser::parse(
        reinterpret_cast<const uint8_t *>(json),
        std::strlen(json),
        evt);
}

void test_parse_valid_payload(void)
{
    OrderEvent evt;
    bool ok = parseStr(R"({"order":42,"status":"ready","eta":"5 min"})", evt);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_INT(42, evt.order);
    TEST_ASSERT_EQUAL_STRING("ready", evt.status);
    TEST_ASSERT_EQUAL_STRING("5 min", evt.eta);
}

void test_parse_empty_object_uses_defaults(void)
{
    OrderEvent evt;
    bool ok = parseStr("{}", evt);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_INT(0, evt.order);
    TEST_ASSERT_EQUAL_STRING("unknown", evt.status);
    TEST_ASSERT_EQUAL_STRING("-", evt.eta);
}

void test_parse_missing_eta_uses_default(void)
{
    OrderEvent evt;
    bool ok = parseStr(R"({"order":7,"status":"ready"})", evt);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_INT(7, evt.order);
    TEST_ASSERT_EQUAL_STRING("ready", evt.status);
    TEST_ASSERT_EQUAL_STRING("-", evt.eta);
}

void test_parse_malformed_json_returns_false(void)
{
    OrderEvent evt;
    bool ok = parseStr("{not valid", evt);
    TEST_ASSERT_FALSE(ok);
}

void test_parse_empty_buffer_returns_false(void)
{
    OrderEvent evt;
    bool ok = parseStr("", evt);
    TEST_ASSERT_FALSE(ok);
}

void test_parse_long_status_is_truncated(void)
{
    OrderEvent evt;
    bool ok = parseStr(R"({"status":"this_is_way_too_long_for_buffer"})", evt);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(15, std::strlen(evt.status));
    TEST_ASSERT_EQUAL_CHAR('\0', evt.status[15]);
}

// --- runner -----------------------------------------------------------------

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_parse_valid_payload);
    RUN_TEST(test_parse_empty_object_uses_defaults);
    RUN_TEST(test_parse_missing_eta_uses_default);
    RUN_TEST(test_parse_malformed_json_returns_false);
    RUN_TEST(test_parse_empty_buffer_returns_false);
    RUN_TEST(test_parse_long_status_is_truncated);
    return UNITY_END();
}

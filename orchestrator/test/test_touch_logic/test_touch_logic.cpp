#include <unity.h>
#include "TouchLogic.h"

using namespace TouchLogic;

void setUp(void) {}
void tearDown(void) {}

// --- hitTest ----------------------------------------------------------------

void test_hitTest_inside_button(void)
{
    Button b = {10, 10, 100, 50, 0};
    TEST_ASSERT_TRUE(hitTest(b, 50, 30));
}

void test_hitTest_outside_left(void)
{
    Button b = {10, 10, 100, 50, 0};
    TEST_ASSERT_FALSE(hitTest(b, 5, 30));
}

void test_hitTest_outside_right(void)
{
    Button b = {10, 10, 100, 50, 0};
    TEST_ASSERT_FALSE(hitTest(b, 110, 30)); // right edge is exclusive
}

void test_hitTest_outside_above(void)
{
    Button b = {10, 10, 100, 50, 0};
    TEST_ASSERT_FALSE(hitTest(b, 50, 5));
}

void test_hitTest_outside_below(void)
{
    Button b = {10, 10, 100, 50, 0};
    TEST_ASSERT_FALSE(hitTest(b, 50, 60)); // bottom edge is exclusive
}

void test_hitTest_top_left_corner_inclusive(void)
{
    Button b = {10, 10, 100, 50, 0};
    TEST_ASSERT_TRUE(hitTest(b, 10, 10));
}

// --- resolveTouch on TABLE_LIST --------------------------------------------

void test_resolveTouch_picks_first_table(void)
{
    // TABLE_BTNS[0] = {30, 60, 110, 60}
    Action a = resolveTouch(Screen::TABLE_LIST, 50, 80);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(ActionType::SELECT_TABLE),
                          static_cast<int>(a.type));
    TEST_ASSERT_EQUAL_INT(1, a.tableId);
}

void test_resolveTouch_picks_second_table(void)
{
    // TABLE_BTNS[1] = {180, 60, 110, 60}
    Action a = resolveTouch(Screen::TABLE_LIST, 200, 80);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(ActionType::SELECT_TABLE),
                          static_cast<int>(a.type));
    TEST_ASSERT_EQUAL_INT(2, a.tableId);
}

void test_resolveTouch_picks_fourth_table(void)
{
    // TABLE_BTNS[3] = {180, 150, 110, 60}
    Action a = resolveTouch(Screen::TABLE_LIST, 200, 180);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(ActionType::SELECT_TABLE),
                          static_cast<int>(a.type));
    TEST_ASSERT_EQUAL_INT(4, a.tableId);
}

void test_resolveTouch_outside_buttons_is_NONE(void)
{
    Action a = resolveTouch(Screen::TABLE_LIST, 0, 0);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(ActionType::NONE),
                          static_cast<int>(a.type));
}

// --- resolveTouch on TABLE_DETAIL ------------------------------------------

void test_resolveTouch_detail_back_button(void)
{
    // BACK_BTN = {5, 5, 70, 35}
    Action a = resolveTouch(Screen::TABLE_DETAIL, 20, 20);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(ActionType::GO_BACK),
                          static_cast<int>(a.type));
}

void test_resolveTouch_detail_status_pending(void)
{
    // STATUS_BTNS[0] = {60, 60, 200, 40}
    Action a = resolveTouch(Screen::TABLE_DETAIL, 100, 70);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(ActionType::SELECT_STATUS),
                          static_cast<int>(a.type));
    TEST_ASSERT_EQUAL_INT(0, a.statusIdx);
}

void test_resolveTouch_detail_status_preparing(void)
{
    // STATUS_BTNS[1] = {60, 110, 200, 40}
    Action a = resolveTouch(Screen::TABLE_DETAIL, 100, 120);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(ActionType::SELECT_STATUS),
                          static_cast<int>(a.type));
    TEST_ASSERT_EQUAL_INT(1, a.statusIdx);
}

void test_resolveTouch_detail_status_ready(void)
{
    // STATUS_BTNS[2] = {60, 160, 200, 40}
    Action a = resolveTouch(Screen::TABLE_DETAIL, 100, 170);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(ActionType::SELECT_STATUS),
                          static_cast<int>(a.type));
    TEST_ASSERT_EQUAL_INT(2, a.statusIdx);
}

void test_resolveTouch_detail_dead_zone_is_NONE(void)
{
    Action a = resolveTouch(Screen::TABLE_DETAIL, 300, 30);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(ActionType::NONE),
                          static_cast<int>(a.type));
}

// --- resolveTouch on SENT_OVERLAY ------------------------------------------

void test_resolveTouch_overlay_ignores_everything(void)
{
    // overlay is the "ENVIADO" confirmation; clicks shouldn't do anything
    Action a = resolveTouch(Screen::SENT_OVERLAY, 100, 100);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(ActionType::NONE),
                          static_cast<int>(a.type));
}

// --- string contract -------------------------------------------------------

void test_status_labels_are_english_for_mqtt(void)
{
    // Subscribers expect English status keys — translating these
    // would break the wire protocol.
    TEST_ASSERT_EQUAL_STRING("pending",   STATUS_LABELS[0]);
    TEST_ASSERT_EQUAL_STRING("preparing", STATUS_LABELS[1]);
    TEST_ASSERT_EQUAL_STRING("ready",     STATUS_LABELS[2]);
}

// --- runner -----------------------------------------------------------------

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_hitTest_inside_button);
    RUN_TEST(test_hitTest_outside_left);
    RUN_TEST(test_hitTest_outside_right);
    RUN_TEST(test_hitTest_outside_above);
    RUN_TEST(test_hitTest_outside_below);
    RUN_TEST(test_hitTest_top_left_corner_inclusive);
    RUN_TEST(test_resolveTouch_picks_first_table);
    RUN_TEST(test_resolveTouch_picks_second_table);
    RUN_TEST(test_resolveTouch_picks_fourth_table);
    RUN_TEST(test_resolveTouch_outside_buttons_is_NONE);
    RUN_TEST(test_resolveTouch_detail_back_button);
    RUN_TEST(test_resolveTouch_detail_status_pending);
    RUN_TEST(test_resolveTouch_detail_status_preparing);
    RUN_TEST(test_resolveTouch_detail_status_ready);
    RUN_TEST(test_resolveTouch_detail_dead_zone_is_NONE);
    RUN_TEST(test_resolveTouch_overlay_ignores_everything);
    RUN_TEST(test_status_labels_are_english_for_mqtt);
    return UNITY_END();
}

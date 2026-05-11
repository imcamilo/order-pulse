#include <unity.h>
#include "StatusLogic.h"

void setUp(void) {}
void tearDown(void) {}

// --- rgbForStatus -----------------------------------------------------------

void test_rgb_pending_is_blue(void)
{
    StatusLogic::RgbColor c = StatusLogic::rgbForStatus("pending");
    TEST_ASSERT_EQUAL_UINT8(0,   c.r);
    TEST_ASSERT_EQUAL_UINT8(0,   c.g);
    TEST_ASSERT_EQUAL_UINT8(255, c.b);
}

void test_rgb_preparing_is_orange(void)
{
    StatusLogic::RgbColor c = StatusLogic::rgbForStatus("preparing");
    TEST_ASSERT_EQUAL_UINT8(255, c.r);
    TEST_ASSERT_EQUAL_UINT8(100, c.g);
    TEST_ASSERT_EQUAL_UINT8(0,   c.b);
}

void test_rgb_ready_is_green(void)
{
    StatusLogic::RgbColor c = StatusLogic::rgbForStatus("ready");
    TEST_ASSERT_EQUAL_UINT8(0,   c.r);
    TEST_ASSERT_EQUAL_UINT8(255, c.g);
    TEST_ASSERT_EQUAL_UINT8(0,   c.b);
}

void test_rgb_unknown_status_is_red(void)
{
    StatusLogic::RgbColor c = StatusLogic::rgbForStatus("garbage");
    TEST_ASSERT_EQUAL_UINT8(255, c.r);
    TEST_ASSERT_EQUAL_UINT8(0,   c.g);
    TEST_ASSERT_EQUAL_UINT8(0,   c.b);
}

void test_rgb_empty_string_is_red(void)
{
    StatusLogic::RgbColor c = StatusLogic::rgbForStatus("");
    TEST_ASSERT_EQUAL_UINT8(255, c.r);
}

// --- readyMotif --------------------------------------------------------------

void test_ready_motif_is_not_empty(void)
{
    int len = 0;
    const StatusLogic::Note *seq = StatusLogic::readyMotif(len);
    TEST_ASSERT_NOT_NULL(seq);
    TEST_ASSERT_GREATER_THAN(0, len);
}

void test_ready_motif_starts_with_G6(void)
{
    int len = 0;
    const StatusLogic::Note *seq = StatusLogic::readyMotif(len);
    TEST_ASSERT_EQUAL_UINT32(1568, seq[0].freq); // G6
}

void test_ready_motif_ends_with_held_Eb6(void)
{
    int len = 0;
    const StatusLogic::Note *seq = StatusLogic::readyMotif(len);
    TEST_ASSERT_EQUAL_UINT32(1244, seq[len - 1].freq);    // Eb6
    TEST_ASSERT_GREATER_THAN(500, seq[len - 1].durMs);    // held note
}

// --- runner -----------------------------------------------------------------

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_rgb_pending_is_blue);
    RUN_TEST(test_rgb_preparing_is_orange);
    RUN_TEST(test_rgb_ready_is_green);
    RUN_TEST(test_rgb_unknown_status_is_red);
    RUN_TEST(test_rgb_empty_string_is_red);
    RUN_TEST(test_ready_motif_is_not_empty);
    RUN_TEST(test_ready_motif_starts_with_G6);
    RUN_TEST(test_ready_motif_ends_with_held_Eb6);
    return UNITY_END();
}

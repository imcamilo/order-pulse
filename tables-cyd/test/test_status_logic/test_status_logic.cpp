#include <unity.h>
#include <cstring>
#include "StatusLogic.h"

void setUp(void) {}
void tearDown(void) {}

// --- rgbForStatus -----------------------------------------------------------

void test_rgb_pending_is_blue_only(void)
{
    StatusLogic::RgbTriplet t = StatusLogic::rgbForStatus("pending");
    TEST_ASSERT_FALSE(t.r);
    TEST_ASSERT_FALSE(t.g);
    TEST_ASSERT_TRUE(t.b);
}

void test_rgb_preparing_is_red_plus_green(void)
{
    // yellow = R + G on
    StatusLogic::RgbTriplet t = StatusLogic::rgbForStatus("preparing");
    TEST_ASSERT_TRUE(t.r);
    TEST_ASSERT_TRUE(t.g);
    TEST_ASSERT_FALSE(t.b);
}

void test_rgb_ready_is_green_only(void)
{
    StatusLogic::RgbTriplet t = StatusLogic::rgbForStatus("ready");
    TEST_ASSERT_FALSE(t.r);
    TEST_ASSERT_TRUE(t.g);
    TEST_ASSERT_FALSE(t.b);
}

void test_rgb_unknown_is_red_only(void)
{
    StatusLogic::RgbTriplet t = StatusLogic::rgbForStatus("garbage");
    TEST_ASSERT_TRUE(t.r);
    TEST_ASSERT_FALSE(t.g);
    TEST_ASSERT_FALSE(t.b);
}

// --- visualForStatus --------------------------------------------------------

void test_visual_pending(void)
{
    StatusLogic::Visual v = StatusLogic::visualForStatus("pending");
    TEST_ASSERT_EQUAL_STRING("PENDIENTE", v.label);
    TEST_ASSERT_EQUAL_UINT16(0x001F, v.color); // blue
}

void test_visual_preparing(void)
{
    StatusLogic::Visual v = StatusLogic::visualForStatus("preparing");
    TEST_ASSERT_EQUAL_STRING("PREPARANDO", v.label);
    TEST_ASSERT_EQUAL_UINT16(0xFD20, v.color); // orange
}

void test_visual_ready(void)
{
    StatusLogic::Visual v = StatusLogic::visualForStatus("ready");
    TEST_ASSERT_EQUAL_STRING("LISTO", v.label);
    TEST_ASSERT_EQUAL_UINT16(0x03E0, v.color); // dark green
}

void test_visual_unknown(void)
{
    StatusLogic::Visual v = StatusLogic::visualForStatus("garbage");
    TEST_ASSERT_EQUAL_STRING("DESCONOCIDO", v.label);
    TEST_ASSERT_EQUAL_UINT16(0xF800, v.color); // red
}

// --- sequenceForStatus ------------------------------------------------------

void test_sequence_pending_is_single_beep(void)
{
    int len = 0;
    const StatusLogic::Note *seq = StatusLogic::sequenceForStatus("pending", len);
    TEST_ASSERT_NOT_NULL(seq);
    TEST_ASSERT_EQUAL_INT(1, len);
}

void test_sequence_preparing_has_two_beeps(void)
{
    int len = 0;
    const StatusLogic::Note *seq = StatusLogic::sequenceForStatus("preparing", len);
    TEST_ASSERT_NOT_NULL(seq);
    TEST_ASSERT_EQUAL_INT(3, len); // beep, silence, beep
}

void test_sequence_ready_is_beethoven_motif(void)
{
    int len = 0;
    const StatusLogic::Note *seq = StatusLogic::sequenceForStatus("ready", len);
    TEST_ASSERT_NOT_NULL(seq);
    TEST_ASSERT_EQUAL_INT(7, len);
    // First sounding note is G6 = 1568 Hz
    TEST_ASSERT_EQUAL_UINT32(1568, seq[0].freq);
    // Last note is held Eb6 = 1244 Hz
    TEST_ASSERT_EQUAL_UINT32(1244, seq[6].freq);
}

void test_sequence_unknown_returns_null(void)
{
    int len = 99;
    const StatusLogic::Note *seq = StatusLogic::sequenceForStatus("garbage", len);
    TEST_ASSERT_NULL(seq);
    TEST_ASSERT_EQUAL_INT(0, len);
}

// --- runner -----------------------------------------------------------------

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_rgb_pending_is_blue_only);
    RUN_TEST(test_rgb_preparing_is_red_plus_green);
    RUN_TEST(test_rgb_ready_is_green_only);
    RUN_TEST(test_rgb_unknown_is_red_only);
    RUN_TEST(test_visual_pending);
    RUN_TEST(test_visual_preparing);
    RUN_TEST(test_visual_ready);
    RUN_TEST(test_visual_unknown);
    RUN_TEST(test_sequence_pending_is_single_beep);
    RUN_TEST(test_sequence_preparing_has_two_beeps);
    RUN_TEST(test_sequence_ready_is_beethoven_motif);
    RUN_TEST(test_sequence_unknown_returns_null);
    return UNITY_END();
}

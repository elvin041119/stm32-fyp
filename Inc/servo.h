typedef struct {
    uint8_t tim_channel;  // e.g., TIM_CHANNEL_1
    TIM_TypeDef* tim;     // e.g., TIM2
    float center_offset; // e.g., -2.5 if 90° is slightly off mechanically
    float min_angle;
    float max_angle;
    uint8_t invert;      // 1 if reversed, 0 if normal
} ServoConfig;

ServoConfig servos[6] = {
    // ID 0: L_HIP_PITCH
    {TIM_CHANNEL_1, TIM2, 0.0, 20.0, 160.0, 0},
    // ID 1: L_KNEE
    {TIM_CHANNEL_2, TIM2, 1.5, 30.0, 150.0, 1}, // inverted because it mirrors right

};

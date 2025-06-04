#include "button.h"

#include <button_gpio.h>
#include <esp_log.h>


static const char* TAG = "Button";
#if BUTTON_VERSION_MAJOR == 3

#define TAG "Button"

#if CONFIG_SOC_ADC_SUPPORTED
AdcButton::AdcButton(const button_adc_config_t& adc_config) : Button(nullptr) {
    button_config_t btn_config = {
        .long_press_time = 2000,
        .short_press_time = 0,
    };
    ESP_ERROR_CHECK(iot_button_new_adc_device(&btn_config, &adc_config, &button_handle_));
}
#endif

Button::Button(button_handle_t button_handle) : button_handle_(button_handle) {
}

Button::Button(gpio_num_t gpio_num, bool active_high, uint16_t long_press_time, uint16_t short_press_time) : gpio_num_(gpio_num) {
    if (gpio_num == GPIO_NUM_NC) {
        return;
    }
    button_config_t button_config = {
        .long_press_time = long_press_time,
        .short_press_time = short_press_time
    };
    button_gpio_config_t gpio_config = {
        .gpio_num = gpio_num,
        .active_level = static_cast<uint8_t>(active_high ? 1 : 0),
        .enable_power_save = false,
        .disable_pull = false
    };
    ESP_ERROR_CHECK(iot_button_new_gpio_device(&button_config, &gpio_config, &button_handle_));
}

Button::~Button() {
    if (button_handle_ != NULL) {
        iot_button_delete(button_handle_);
    }
}

void Button::OnPressDown(std::function<void()> callback) {
    if (button_handle_ == nullptr) {
        return;
    }
    on_press_down_ = callback;
    iot_button_register_cb(button_handle_, BUTTON_PRESS_DOWN, nullptr, [](void* handle, void* usr_data) {
        Button* button = static_cast<Button*>(usr_data);
        if (button->on_press_down_) {
            button->on_press_down_();
        }
    }, this);
}

void Button::OnPressUp(std::function<void()> callback) {
    if (button_handle_ == nullptr) {
        return;
    }
    on_press_up_ = callback;
    iot_button_register_cb(button_handle_, BUTTON_PRESS_UP, nullptr, [](void* handle, void* usr_data) {
        Button* button = static_cast<Button*>(usr_data);
        if (button->on_press_up_) {
            button->on_press_up_();
        }
    }, this);
}

void Button::OnLongPress(std::function<void()> callback) {
    if (button_handle_ == nullptr) {
        return;
    }
    on_long_press_ = callback;
    iot_button_register_cb(button_handle_, BUTTON_LONG_PRESS_START, nullptr, [](void* handle, void* usr_data) {
        Button* button = static_cast<Button*>(usr_data);
        if (button->on_long_press_) {
            button->on_long_press_();
        }
    }, this);
}

void Button::OnClick(std::function<void()> callback) {
    if (button_handle_ == nullptr) {
        return;
    }
    on_click_ = callback;
    iot_button_register_cb(button_handle_, BUTTON_SINGLE_CLICK, nullptr, [](void* handle, void* usr_data) {
        Button* button = static_cast<Button*>(usr_data);
        if (button->on_click_) {
            button->on_click_();
        }
    }, this);
}

void Button::OnDoubleClick(std::function<void()> callback) {
    if (button_handle_ == nullptr) {
        return;
    }
    on_double_click_ = callback;
    iot_button_register_cb(button_handle_, BUTTON_DOUBLE_CLICK, nullptr, [](void* handle, void* usr_data) {
        Button* button = static_cast<Button*>(usr_data);
        if (button->on_double_click_) {
            button->on_double_click_();
        }
    }, this);
}


#elif BUTTON_VERSION_MAJOR >= 4

#if CONFIG_SOC_ADC_SUPPORTED
Button::Button(const button_adc_config_t& adc_cfg) {
    button_config_t button_config = {
        .long_press_time = 1000,
        .short_press_time = 50,
    };
    esp_err_t err = iot_button_new_adc_device(&button_config, &adc_cfg, &button_handle_);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ADC Button create failed");
        return;
    }
}
#endif
Button::Button(gpio_num_t gpio_num, bool active_high) : gpio_num_(gpio_num) {
    if (gpio_num == GPIO_NUM_NC) {
        return;
    }
    button_gpio_config_t btn_gpio_cfg = { 
        .gpio_num = gpio_num,
        .active_level = static_cast<uint8_t>(active_high ? 1 : 0) 
        };
    button_config_t button_config = {
        .long_press_time = 1000,
        .short_press_time = 50,
    };
    esp_err_t ret = iot_button_new_gpio_device(&button_config, &btn_gpio_cfg, &button_handle_);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "GPIO Button create failed");
        return;
    }
}

Button::~Button() {
    if (button_handle_ != NULL) {
        iot_button_delete(button_handle_);
    }
}

void Button::OnPressDown(std::function<void()> callback) {
    if (button_handle_ == nullptr) {
        return;
    }
    on_press_down_ = callback;
    iot_button_register_cb(button_handle_, BUTTON_PRESS_DOWN, NULL, [](void* handle, void* usr_data) {
        Button* button = static_cast<Button*>(usr_data);
        if (button->on_press_down_) {
            button->on_press_down_();
        } 
    }, this);
}

void Button::OnPressUp(std::function<void()> callback) {
    if (button_handle_ == nullptr) {
        return;
    }
    on_press_up_ = callback;
    iot_button_register_cb(button_handle_, BUTTON_PRESS_UP, NULL, [](void* handle, void* usr_data) {
        Button* button = static_cast<Button*>(usr_data);
        if (button->on_press_up_) {
            button->on_press_up_();
        } 
    }, this);
}

void Button::OnLongPress(std::function<void()> callback) {
    if (button_handle_ == nullptr) {
        return;
    }
    on_long_press_ = callback;
    iot_button_register_cb(button_handle_, BUTTON_LONG_PRESS_START, NULL, [](void* handle, void* usr_data) {
        Button* button = static_cast<Button*>(usr_data);
        if (button->on_long_press_) {
            button->on_long_press_();
        } 
    }, this);
}

void Button::OnClick(std::function<void()> callback) {
    if (button_handle_ == nullptr) {
        return;
    }
    on_click_ = callback;
    iot_button_register_cb(button_handle_, BUTTON_SINGLE_CLICK, NULL, [](void* handle, void* usr_data) {
        Button* button = static_cast<Button*>(usr_data);
        if (button->on_click_) {
            button->on_click_();
        } 
    }, this);
}

void Button::OnDoubleClick(std::function<void()> callback) {
    if (button_handle_ == nullptr) {
        return;
    }
    on_double_click_ = callback;
    iot_button_register_cb(button_handle_, BUTTON_DOUBLE_CLICK, NULL, [](void* handle, void* usr_data) {
        Button* button = static_cast<Button*>(usr_data);
        if (button->on_double_click_) {
            button->on_double_click_();
        } 
    }, this);
}
#endif

void Button::OnMultipleClick(std::function<void()> callback, uint8_t click_count) {
    if (button_handle_ == nullptr) {
        return;
    }
    on_multiple_click_ = callback;
    button_event_args_t event_args = {
        .multiple_clicks = {
            .clicks = click_count
        }
    };
    iot_button_register_cb(button_handle_, BUTTON_MULTIPLE_CLICK, &event_args, [](void* handle, void* usr_data) {
        Button* button = static_cast<Button*>(usr_data);
        if (button->on_multiple_click_) {
            button->on_multiple_click_();
        }
    }, this);
}


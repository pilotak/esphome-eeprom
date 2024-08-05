import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import i2c
from esphome.const import CONF_ID, CONF_SIZE

DEPENDENCIES = ["i2c"]
MULTI_CONF = True

CONF_ON_SETUP = "on_setup"
EEPROM_SIZES = {
    "512KB": 65536,
    "256KB": 32768,
    "128KB": 16384,
    "64KB": 8192,
    "32KB": 4096,
    "16KB": 2048,
    "8KB": 1024,
    "4KB": 512,
    "2KB": 256,
    "1KB": 128,
}


eeprom_ns = cg.esphome_ns.namespace("i2c_eeprom")
EEPROMComponent = eeprom_ns.class_("I2C_EEPROM", cg.Component, i2c.I2CDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(EEPROMComponent),
            cv.Required(CONF_SIZE): cv.enum(EEPROM_SIZES, upper=True),
            cv.Optional(CONF_ON_SETUP): automation.validate_automation(single=True),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x50))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_size(config[CONF_SIZE]))
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    # Handle on_setup lambda
    if CONF_ON_SETUP in config:
        await automation.build_automation(
            var.get_on_setup_trigger(), [], config[CONF_ON_SETUP]
        )

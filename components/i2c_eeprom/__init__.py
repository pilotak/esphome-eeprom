import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID, CONF_TYPE, CONF_SIZE

DEPENDENCIES = ["i2c"]
MULTI_CONF = True

eeprom_ns = cg.esphome_ns.namespace("i2c_eeprom")
EEPROMComponent = eeprom_ns.class_("I2C_EEPROM", cg.Component, i2c.I2CDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(EEPROMComponent),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x50))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

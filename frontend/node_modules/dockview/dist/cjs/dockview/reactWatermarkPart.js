"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.ReactWatermarkPart = void 0;
var react_1 = require("../react");
var ReactWatermarkPart = /** @class */ (function () {
    function ReactWatermarkPart(id, component, reactPortalStore) {
        this.id = id;
        this.component = component;
        this.reactPortalStore = reactPortalStore;
        this._element = document.createElement('div');
        this._element.className = 'dv-react-part';
        this._element.style.height = '100%';
        this._element.style.width = '100%';
    }
    Object.defineProperty(ReactWatermarkPart.prototype, "element", {
        get: function () {
            return this._element;
        },
        enumerable: false,
        configurable: true
    });
    ReactWatermarkPart.prototype.init = function (parameters) {
        this.part = new react_1.ReactPart(this.element, this.reactPortalStore, this.component, {
            group: parameters.group,
            containerApi: parameters.containerApi,
        });
    };
    ReactWatermarkPart.prototype.focus = function () {
        // noop
    };
    ReactWatermarkPart.prototype.update = function (params) {
        var _a, _b, _c;
        if (this.parameters) {
            this.parameters.params = params.params;
        }
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.update({ params: (_c = (_b = this.parameters) === null || _b === void 0 ? void 0 : _b.params) !== null && _c !== void 0 ? _c : {} });
    };
    ReactWatermarkPart.prototype.layout = function (_width, _height) {
        // noop - retrieval from api
    };
    ReactWatermarkPart.prototype.dispose = function () {
        var _a;
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.dispose();
    };
    return ReactWatermarkPart;
}());
exports.ReactWatermarkPart = ReactWatermarkPart;

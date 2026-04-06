"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.ReactPanelHeaderPart = void 0;
var react_1 = require("../react");
var ReactPanelHeaderPart = /** @class */ (function () {
    function ReactPanelHeaderPart(id, component, reactPortalStore) {
        this.id = id;
        this.component = component;
        this.reactPortalStore = reactPortalStore;
        this._element = document.createElement('div');
        this._element.className = 'dv-react-part';
        this._element.style.height = '100%';
        this._element.style.width = '100%';
    }
    Object.defineProperty(ReactPanelHeaderPart.prototype, "element", {
        get: function () {
            return this._element;
        },
        enumerable: false,
        configurable: true
    });
    ReactPanelHeaderPart.prototype.focus = function () {
        //noop
    };
    ReactPanelHeaderPart.prototype.init = function (parameters) {
        this.part = new react_1.ReactPart(this.element, this.reactPortalStore, this.component, {
            params: parameters.params,
            api: parameters.api,
            containerApi: parameters.containerApi,
            tabLocation: parameters.tabLocation,
        });
    };
    ReactPanelHeaderPart.prototype.update = function (event) {
        var _a;
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.update({ params: event.params });
    };
    ReactPanelHeaderPart.prototype.layout = function (_width, _height) {
        // noop - retrieval from api
    };
    ReactPanelHeaderPart.prototype.dispose = function () {
        var _a;
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.dispose();
    };
    return ReactPanelHeaderPart;
}());
exports.ReactPanelHeaderPart = ReactPanelHeaderPart;

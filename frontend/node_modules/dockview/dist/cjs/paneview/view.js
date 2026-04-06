"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.PanePanelSection = void 0;
var react_1 = require("../react");
var PanePanelSection = /** @class */ (function () {
    function PanePanelSection(id, component, reactPortalStore) {
        this.id = id;
        this.component = component;
        this.reactPortalStore = reactPortalStore;
        this._element = document.createElement('div');
        this._element.style.height = '100%';
        this._element.style.width = '100%';
    }
    Object.defineProperty(PanePanelSection.prototype, "element", {
        get: function () {
            return this._element;
        },
        enumerable: false,
        configurable: true
    });
    PanePanelSection.prototype.init = function (parameters) {
        this.part = new react_1.ReactPart(this.element, this.reactPortalStore, this.component, {
            params: parameters.params,
            api: parameters.api,
            title: parameters.title,
            containerApi: parameters.containerApi,
        });
    };
    PanePanelSection.prototype.toJSON = function () {
        return {
            id: this.id,
        };
    };
    PanePanelSection.prototype.update = function (params) {
        var _a;
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.update(params.params);
    };
    PanePanelSection.prototype.dispose = function () {
        var _a;
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.dispose();
    };
    return PanePanelSection;
}());
exports.PanePanelSection = PanePanelSection;

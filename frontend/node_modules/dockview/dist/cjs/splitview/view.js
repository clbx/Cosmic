"use strict";
var __extends = (this && this.__extends) || (function () {
    var extendStatics = function (d, b) {
        extendStatics = Object.setPrototypeOf ||
            ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
            function (d, b) { for (var p in b) if (Object.prototype.hasOwnProperty.call(b, p)) d[p] = b[p]; };
        return extendStatics(d, b);
    };
    return function (d, b) {
        if (typeof b !== "function" && b !== null)
            throw new TypeError("Class extends value " + String(b) + " is not a constructor or null");
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
exports.ReactPanelView = void 0;
var dockview_core_1 = require("dockview-core");
var react_1 = require("../react");
var ReactPanelView = /** @class */ (function (_super) {
    __extends(ReactPanelView, _super);
    function ReactPanelView(id, component, reactComponent, reactPortalStore) {
        var _this = _super.call(this, id, component) || this;
        _this.reactComponent = reactComponent;
        _this.reactPortalStore = reactPortalStore;
        return _this;
    }
    ReactPanelView.prototype.getComponent = function () {
        var _a, _b;
        return new react_1.ReactPart(this.element, this.reactPortalStore, this.reactComponent, {
            params: (_b = (_a = this._params) === null || _a === void 0 ? void 0 : _a.params) !== null && _b !== void 0 ? _b : {},
            api: this.api,
            containerApi: new dockview_core_1.SplitviewApi(this._params.accessor),
        });
    };
    return ReactPanelView;
}(dockview_core_1.SplitviewPanel));
exports.ReactPanelView = ReactPanelView;
